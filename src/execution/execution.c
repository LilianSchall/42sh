#include "execution.h"

// used to know the value of break command and how many loop we need to skip
static int break_val = 0;
// used to know the value of continue command and how many loop we need to skip
static int continue_val = 0;
// used to know the number of loop that we are in (the outermost enclosing loop)
static int nb_loop = 0;


char *execute_AST_D_SUBSHELL(struct AST *tree) 
{
    int pipefd[2];
    pipe(pipefd);

    int pid = fork();
    if (pid == 0) 
    {
        // child process
        close(pipefd[0]);

        int save1 = dup(STDOUT_FILENO);

        dup2(pipefd[1], STDOUT_FILENO);
        struct AST *child = tree->linked_list->head->data;
        execute_AST(child);
        fflush(stdout);

        close(pipefd[1]);
        dup2(save1, STDOUT_FILENO);
        close(save1);
        exit(0);
    } 
    else 
    {
        // parent process
        char * result = get_content_of_pipe(pipefd);
        wait(NULL);
        fprintf(stderr,"debug: got string '%s'\n", result);
        return result;
    }
}

static int not_builtin_fn(int argc, char **argv)
{
    if (argc == 0)
        return 0;

    int ret_val = 0;
    int pid = fork();

    if (!pid) // child goes in
    {
        int status_code = execvp(argv[0], argv);
        if (status_code == -1)
        {
            errx(127, "command not found: %s", argv[0]);
        }
    }
    wait(&ret_val);

    return WEXITSTATUS(ret_val);
}

static int execute_AST_cmd(struct AST *tree)
{
    int ret_val = 0;

    int argc = 0;
    char **argv = new_argv(tree, &argc);

    if (!strcmp("echo", argv[0])) // builtin command
    {
        ret_val = echo_fn(argc, argv);
    }
    else if (!strcmp("true", argv[0])) // true
    {
        ret_val = true_fn(argc, argv);
    }
    else if (!strcmp("false", argv[0])) // false
    {
        ret_val = false_fn(argc, argv);
    }
    else if (!strcmp("break", argv[0])) // false
    {
        ret_val = exec_break_continue(argc, argv, &break_val, nb_loop);
    }
    else if (!strcmp("continue", argv[0])) // false
    {
        ret_val = exec_break_continue(argc, argv, &continue_val, nb_loop);
    }
    else
    {
        ret_val = not_builtin_fn(argc, argv); // not a builtin command
    }

    free_argv(argc, argv);

    return ret_val;
}

static int execute_AST_if(struct AST *tree)
{
    struct linked_node *child = tree->linked_list->head;
    struct AST *cond = child->data;
    child = child->next;
    struct AST *t = child->data;
    if (execute_AST(cond) == 0) // cond is true, we execute the 'then'
    {
        return execute_AST(t);
    }
    else
    {
        if (child->next) // cond is false, we execute the 'else'
            return execute_AST(child->next->data);
    }
    return 0;
}

static int execute_AST_subshell(struct AST *tree)
{
    int ret_val = 0;
    int pid = fork();

    if (!pid) // child goes in
    {
        struct AST *child = tree->linked_list->head->data;
        ret_val = execute_AST(child);
        exit(ret_val);

    }
    wait(&ret_val);
    if(WIFEXITED(ret_val))
        return WEXITSTATUS(ret_val);
    return 0;
    
}

// exec a while or until command
// if val_cond = 0 -> while
// if val_cond = 1 -> until
static int execute_AST_while_until(struct AST *tree, int val_cond)
{
    int return_val = 0;
    int while_cond = val_cond;

    struct linked_node *child = tree->linked_list->head;
    struct AST *cond = child->data;

    child = child->next;
    struct AST *bloc = child->data;

    while (while_cond == val_cond && break_val == 0 && continue_val == 0)
    {
        while_cond = execute_AST(cond); // check condition

        if (while_cond == val_cond)
            return_val = execute_AST(bloc); // exec commands
        
        if (continue_val > 0)
            continue_val--;
    }

    if (break_val > 0)
        break_val--;

    return return_val;
}

static int execute_AST_redirection(struct AST *tree)
{
    int return_val = 0;
    enum token_type r_type = tree->value->type;

    struct linked_node *child_list = tree->linked_list->head;

    struct AST *ast_from = child_list->data;
    struct AST *ast_exec = child_list->next->data;
    struct AST *ast_to = child_list->next->next->data;

    int fd_from = get_fd_from_ast(ast_from, r_type);
    if (fd_from == -1)
        return 2;

    int fd_to = get_fd_from_ast(ast_to, r_type);
    if (fd_to == -1)
    {
        close_fd(fd_from, ast_from);
        return 2;
    }

    return_val = redirection_fd_to_fd(ast_exec, fd_from, fd_to);

    close_fd(fd_from, ast_from);
    close_fd(fd_to, ast_to);

    return return_val;
}

static int execute_AST_for(struct AST *tree)
{
    int ret_val = 0;
    struct linked_node *child = tree->linked_list->head;
    struct AST *ast_arg = child->data;
    char *var_name = ast_arg->value->symbol;
    child = child->next; // should not be NULL (check here if error occurs)
    struct AST *ast_iter_seq = child->data;
    child = child->next; // should not be NULL either
    struct AST *ast_seq = child->data;

    if (ast_iter_seq->type == ITER)
    {
        struct linked_node *iter_child = ast_iter_seq->linked_list->head;
        while (iter_child && break_val == 0 && continue_val == 0)
        {
            struct AST *iter_arg = iter_child->data;
            setenv(var_name, iter_arg->value->symbol, 1);
            ret_val = execute_AST(ast_seq);
            iter_child = iter_child->next;

            if (continue_val > 0)
                continue_val--;
        }

        if (break_val > 0)
            break_val--;
    }
    else // the tree is a SEQUENCE, need to exec in a subshell
    {
        // TODO in step 3
    }
    return ret_val;
}

static int execute_AST_operator(struct AST *tree)
{
    char *op = tree->value->symbol;
    int ret_val = 0;

    struct linked_node *node = tree->linked_list->head;
    struct AST *child = node->data;
    struct AST *child2;

    if (node->next)
        child2 = node->next->data;

    if (!strcmp("!", op)) // ! condition
    {
        ret_val = !execute_AST(child);
    }
    else if (!strcmp("&&", op)) // && condition
    {
        ret_val = execute_AST(child);

        if (ret_val == 1)
            return 1;

        return execute_AST(child2);
    }
    else if (!strcmp("||", op)) // || condition
    {
        ret_val = execute_AST(child);
        if (ret_val == 0)
            return 0;

        return execute_AST(child2);
    }

    return ret_val;
}

static int execute_AST_assignment(struct AST *tree)
{
    int ret_val = 1;
    struct linked_node *child = tree->linked_list->head;
    struct AST *var_name_ast = child->data;
    // variable name is the token value of the ast
    char *var_name = var_name_ast->value->symbol;

    // taking second child(cant be NULL)
    struct AST *var_value_ast = child->next->data;

    if (var_value_ast->type == ARG)
    {
        ret_val = setenv(var_name, var_value_ast->value->symbol, 1);
    }
    else // the child is a sequence -> subshell and take stdout as value
    {
        // TODO in step 3 or 4
    }
    return ret_val;
}

static int execute_AST_condition(struct AST *tree)
{
    int ret_val = 0;
    switch (tree->value->type)
    {
    case IF:
        ret_val = execute_AST_if(tree);
        break;
    case WHILE:
        nb_loop++;
        ret_val = execute_AST_while_until(tree, 0); // while is true
        nb_loop--;
        break;
    case UNTIL:
        nb_loop++;
        ret_val = execute_AST_while_until(tree, 1); // until is true
        nb_loop--;
        break;
    case FOR:
        nb_loop++;
        ret_val = execute_AST_for(tree);
        nb_loop--;
        break;
    default:
        break;
    }
    return ret_val;
}

static int execute_AST_sequence(struct AST *tree)
{
    int ret_val = 0;

    if (tree->linked_list == NULL)
        return 0;

    for (struct linked_node *node = tree->linked_list->head; node;
         node = node->next)
    {
        if(break_val != 0 || continue_val != 0)
            return ret_val;

        struct AST *child = node->data;
        ret_val = execute_AST(child);
    }
    return ret_val;
}

int execute_AST(struct AST *tree)
{
    if (!tree)
        return 0;

    int ret_val = 0;

    switch (tree->type)
    {
    case SEQUENCE:
        ret_val = execute_AST_sequence(tree);
        break;
    case SUBSHELL:
        ret_val = execute_AST_subshell(tree);
        break;
    case REDIRECTION:
        ret_val = execute_AST_redirection(tree);
        break;
    case PIPE:
        ret_val = execute_AST_pipe(tree);
        break;
    case COMMAND:
        ret_val = execute_AST_cmd(tree);
        break;
    case OPERATOR:
        ret_val = execute_AST_operator(tree);
        break;
    case CONDITION:
        ret_val = execute_AST_condition(tree);
        break;
    case ASSIGNMENT:
        ret_val = execute_AST_assignment(tree);
        break;
    default:
        break;
    }
    return ret_val;
}

