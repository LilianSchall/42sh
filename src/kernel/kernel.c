#include "kernel.h"

static int launch_interactive_mode(int options);
static int launch_script_mode(int options, char *file_script);

int launch_interactive_mode(int options)
{
    char *content = NULL;
    int status_code = 0;
    int last_status_code = 0;
    do
    {
        printf("42sh$ ");
        fflush(stdout);
        
        // get content from stdin
        content = get_interactive_content();

        // execute given command
        last_status_code = status_code;
        status_code = execute_shell_command(options, content);

        // free and dereference processed content
        free(content);
        content = NULL;
    } while (status_code != -1);
    return last_status_code;
}

int launch_script_mode(int options, char *file_script)
{
    // get commands from script
    char *input = get_file_content(file_script);

    // execute input
    return execute_shell_command(options, input);
}

int launch_shell(int options, char *file_script)
{
    if (!file_script)
        return launch_interactive_mode(options);
    return launch_script_mode(options, file_script);
}

int execute_shell_command(int options, char *input)
{
    // get token_list based on given input
    struct linked_list *token_list = build_token_list(input);

    // build AST based on token_list
    struct AST *tree = build_shell_AST(token_list);
    
    // execute tree
    return execute_AST(tree);
}
