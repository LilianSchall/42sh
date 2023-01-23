#include "builtin/builtin.h"

int check_slash(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '/')
            return 1;
        i++;
    }

    return 0;
}

char *get_filename_dot(char *str)
{
    char *env_path = getenv("PATH");
    char *result;

    if (env_path[strlen(env_path) - 1] == '/')
    {
        result =
            mem_malloc(sizeof(char) * (strlen(env_path) + strlen(str) + 1));
        sprintf(result, "%s%s", env_path, str);
    }
    else
    {
        result =
            mem_malloc(sizeof(char) * (strlen(env_path) + strlen(str) + 2));
        sprintf(result, "%s/%s", env_path, str);
    }
    return result;
}

char **create_new_argv(int argc, char **argv)
{
    char **result = mem_malloc(sizeof(char *) * (argc));

    int i = 1;

    while (i < argc)
    {
        result[i - 1] = mem_malloc(sizeof(char) * (strlen(argv[i]) + 1));
        sprintf(result[i - 1], "%s", argv[i]);
        i++;
    }
    result[i - 1] = NULL;

    return result;
}

int dot_fn(int argc, char **argv, struct linked_list *functions)
{
    // check if a file is specified
    if (argc == 1)
    {
        fprintf(stderr,
                "42sh : .: filename argument required\n.: usage: . filename "
                "[arguments]\n");
        return 2;
    }
    char *input;

    // check if the filename contains a '/'
    // if not, we take it from PATH
    if (check_slash(argv[1]))
        input = get_file_content(argv[1]);
    else
    {
        char *filename = get_filename_dot(argv[1]);
        input = get_file_content(filename);
        mem_free(filename);
    }

    if(strlen(input) == 0)
        return 0;

    if (input == NULL)
    {
        fprintf(stderr, "42sh: file not found\n");
        return 2;
    }

    struct linked_list *tokens = build_token_list(input);

    if (tokens == NULL)
    {
        mem_free(input);
        return 2;
    }

    struct AST *tree = build_shell_AST(tokens);

    if (tree == NULL)
    {
        deep_free_list(tokens, free_token);
        mem_free(input);
        return 2;
    }

    // create new argv
    char **new_argv = create_new_argv(argc, argv);

    int ret_val = execute_AST(tree, new_argv, functions);

    deep_free_list(tokens, free_token);
    free_AST(tree);
    mem_free(input);
    free_argv(new_argv);

    return ret_val;
}
