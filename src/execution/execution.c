#include "execution.h"

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

static int execute_AST_cmd(struct AST *tree, struct env *env)
{
    int ret_val = 0;

    int argc = 0;
    char **argv = new_argv(tree, &argc, env);

    if (!argv[0])
        ret_val = 0;
    else if (!strcmp("echo", argv[0])) // builtin command
    {
        ret_val = echo_fn(argc, argv);
    }
    else if (!strcmp(".", argv[0])) // dot command
    {
        ret_val = dot_fn(argc, argv, env->functions);
    }
    else if (!strcmp("cd", argv[0])) // builtin command
    {
        ret_val = cd_fn(argc, argv);
    }
    else if (!strcmp("export", argv[0])) // builtin command
    {
        ret_val = export_fn(argc, argv);
    }
    else if (!strcmp("unset", argv[0])) // builtin command
    {
        ret_val = unset_fn(argc, argv, env->functions);
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
    else if (call_function(env->functions, argv, &ret_val))
    {}
    else
    {
        ret_val = not_builtin_fn(argc, argv); // not a builtin command
    }

    free_argv(argv);

    char *tmp = mem_malloc(sizeof(char) * 7);
    sprintf(tmp, "%d", ret_val);
    setenv("?", tmp, 1);
    mem_free(tmp);

    return ret_val;
}

static int execute_AST_sequence(struct AST *tree, struct env *env)
{
    int ret_val = 0;

    if (tree->linked_list == NULL)
        return 0;

    for (struct linked_node *node = tree->linked_list->head; node;
         node = node->next)
    {
        if (!check_status() || (status && status->exit_bool == 1))
            return ret_val;

        struct AST *child = node->data;
        ret_val = execute_AST_main(child, env);
    }
    return ret_val;
}

int execute_AST_main(struct AST *tree, struct env *env)
{
    if (!tree)
        return 0;

    int ret_val = 0;

    switch (tree->type)
    {
    case SEQUENCE:
        ret_val = execute_AST_sequence(tree, env);
        break;
    case HERE_DOC:
        ret_val = execute_AST_here_doc(tree, env);
        break;
    case CASE_C:
        ret_val = execute_AST_case(tree, env);
        break;
    case SUBSHELL:
        ret_val = execute_AST_subshell(tree, env);
        break;
    case REDIRECTION:
        ret_val = execute_AST_redirection(tree, env);
        break;
    case PIPE:
        ret_val = execute_AST_pipe(tree, env);
        break;
    case COMMAND:
        ret_val = execute_AST_cmd(tree, env);
        break;
    case OPERATOR:
        ret_val = execute_AST_operator(tree, env);
        break;
    case CONDITION:
        ret_val = execute_AST_condition(tree, env);
        break;
    case ASSIGNMENT:
        ret_val = execute_AST_assignment(tree, env);
        break;
    case FUNCTION:
        ret_val = execute_AST_function(tree, env);
    default:
        break;
    }
    return ret_val;
}

int execute_AST(struct AST *tree, char **argv, struct linked_list *functions)
{
    if (tree == NULL)
        return 0;

    if (!functions)
    {
        setenv("IFS", " \t\n", 1);
        srand(time(NULL));
    }

    struct env env = { .argv = argv,
                       .functions = functions ? functions : new_list() };

    int ret_val = execute_AST_main(tree, &env);

    if (!functions)
        deep_free_list(env.functions, free_function);

    return ret_val;
}
