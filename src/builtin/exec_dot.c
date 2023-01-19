#include "builtin/builtin.h"


int check_slash(char *str)
{
    int i = 0;
    while(str[i] != '\0')
    {
        if(str[i] == '/')
            return 1;
    }

    return 0;
}

char *get_filename_dot(char *str)
{

    char *env_path = getenv("PATH");
    
    char *result = mem_malloc(sizeof(char) * (strlen(env_path) + strlen(str) + 1));

    sprintf(result, "%s%s", env_path, str);

    return result;
}

char **create_new_argv(int argc, char **argv)
{
    char **result = malloc(sizeof(char *) * argc);

    int i = 1;

    while(argv[i] != NULL)
    {
        result[i-1] = copy_string(argv[i]);
        i++;
    }
    return result;
}


int dot_fn(int argc, char **argv)
{
    // check if a file is specified
    if(argc == 1)
    {
        fprintf(stderr,"42sh : .: filename argument required\n.: usage: . filename [arguments]\n");
        return 2;
    }
    char *input;

    // check if the filename contains a '/'
    // if not, we take it from PATH
    if(check_slash(argv[0]))
        input = get_file_content(argv[1]);
    else
    {
        char *filename = get_filename_dot(argv[1]);
        input = get_file_content(filename);
        mem_free(filename);
    }
    
    if(input == NULL)
        return 2;

    struct linked_list *tokens = build_token_list(input);
    
    if(tokens == NULL)
    {
        mem_free(input);
        return 2;
    }

    struct AST *tree = build_shell_AST(tokens);

    if(tree == NULL)
    {

        deep_free_list(tokens, free_token);
        mem_free(input);
        return 2;
    }

    //create new argv
    char **new_argv = create_new_argv(argc, argv);

    int ret_val = execute_AST(tree, new_argv);

    deep_free_list(tokens, free_token);
    free_AST(tree);
    mem_free(input);
    free_argv(new_argv);
    
    return ret_val;
}