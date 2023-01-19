#include "execution.h"

// used to know the value of break command and how many loop we need to skip
int break_val = 0;
// used to know the value of continue command and how many loop we need to skip
int continue_val = 0;
// used to know the number of loop that we are in (the outermost enclosing loop)
int nb_loop = 0;


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
    else if (!strcmp("exit", argv[0])) // exit command
    {
        ret_val = exit_fn(argc, argv);
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
        ret_val = exec_break_continue(argc, argv, &(status->break_val));
    }
    else if (!strcmp("continue", argv[0])) 
    {
        ret_val = exec_break_continue(argc, argv, &(status->continue_val));
    }
    else
    {
        ret_val = not_builtin_fn(argc, argv); // not a builtin command
    }

    free_argv(argv);

    char *tmp = mem_malloc(sizeof(char) * 4);
    sprintf(tmp, "%d", ret_val);
    setenv("?", tmp, 1);
    mem_free(tmp);

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
        if(!check_status() || (status && status->exit_bool == 1))
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

