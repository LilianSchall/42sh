#include "execution.h"

int not_builtin_fn(int argc, char **argv)
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

int execute_AST_cmd(struct AST *tree)
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
    else
    {
        ret_val = not_builtin_fn(argc, argv); // not a builtin command
    }

    free_argv(argc, argv);

    char *tmp = mem_malloc(sizeof(char) * 4);
    sprintf(tmp, "%d", ret_val);
    setenv("?", tmp, 1);
    mem_free(tmp);

    return ret_val;
}

int execute_AST_if(struct AST *tree)
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

// exec a while or until command
// if val_cond = 0 -> while
// if val_cond = 1 -> until
int execute_AST_while_until(struct AST *tree, int val_cond)
{
    int return_val = 0;
    int while_cond = val_cond;

    struct linked_node *child = tree->linked_list->head;
    struct AST *cond = child->data;

    child = child->next;
    struct AST *bloc = child->data;

    while (while_cond == val_cond)
    {
        while_cond = execute_AST(cond); // check condition

        if (while_cond == val_cond)
            return_val = execute_AST(bloc); // exec commands
    }

    return return_val;
}

int execute_AST_redirection(struct AST *tree)
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

int execute_AST_for(struct AST *tree)
{
    int ret_val = 0;
    struct linked_node *child = tree->linked_list->head;
    struct AST *ast_arg = child->data;
    char *var_name = ast_arg->value->values[0]->value;
    
    child = child->next; // should not be NULL (check here if error occurs)
    struct AST *ast_iter = child->data;

    // create iter table, should expand var and subshells
    int argc = 0;
    char **iter_args = new_argv(ast_iter, &argc); 

    child = child->next; // should not be NULL either
    struct AST *ast_seq = child->data;
    int i = 0;
    while (iter_args[i])
    {
        setenv(var_name, iter_args[i], 1);
        ret_val = execute_AST(ast_seq);
        i++;
    }
    return ret_val;
}

int execute_AST_operator(struct AST *tree)
{
    enum token_type type = tree->value->type;
    int ret_val = 0;

    struct linked_node *node = tree->linked_list->head;
    struct AST *child = node->data;
    struct AST *child2;

    if (node->next)
        child2 = node->next->data;

    if (type == NEG) // ! condition
    {
        ret_val = !execute_AST(child);
    }
    else if (type == AND) // && condition
    {
        ret_val = execute_AST(child);

        if (ret_val == 1)
            return 1;

        return execute_AST(child2);
    }
    else if (type == OR) // || condition
    {
        ret_val = execute_AST(child);
        if (ret_val == 0)
            return 0;

        return execute_AST(child2);
    }

    return ret_val;
}

int execute_AST_assignment(struct AST *tree)
{
    int ret_val = 1;
    struct linked_node *child = tree->linked_list->head;
    struct AST *var_name_ast = child->data;
    // variable name is the token value of the ast
    char *var_name = var_name_ast->value->values[0]->value;

    // taking second child(cant be NULL)
    struct AST *var_value_ast = child->next->data;

    if (var_value_ast->type == ARG)
    {
        ret_val = setenv(var_name, var_value_ast->value->values[0]->value, 1);
    }
    else // the child is a sequence -> subshell and take stdout as value
    {
        // TODO in step 3 or 4
    }
    return ret_val;
}

int execute_AST_condition(struct AST *tree)
{
    int ret_val = 0;
    switch (tree->value->type)
    {
    case IF:
        ret_val = execute_AST_if(tree);
        break;
    case WHILE:
        ret_val = execute_AST_while_until(tree, 0); // while is true
        break;
    case UNTIL:
        ret_val = execute_AST_while_until(tree, 1); // until is true
        break;
    case FOR:
        ret_val = execute_AST_for(tree);
        break;
    default:
        break;
    }
    return ret_val;
}

int execute_AST_sequence(struct AST *tree)
{
    int ret_val = 0;

    if (tree->linked_list == NULL)
        return 0;

    for (struct linked_node *node = tree->linked_list->head; node;
         node = node->next)
    {
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

