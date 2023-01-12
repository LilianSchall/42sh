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
        content = get_interactive_content(true);

        // execute given command
        last_status_code = status_code;
        status_code = execute_shell_command(options, content);

        if (last_status_code == -1)
            last_status_code = status_code; // first init of first command

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
    int status_code = execute_shell_command(options, input);
    free(input);

    return status_code;
}

int launch_shell(int options, char *file_script, char *input)
{
    int status_code = 0;

    if (input)
        status_code = execute_shell_command(options, input);
    else if (!file_script)
    {
        if (isatty(STDIN_FILENO))
            status_code = launch_interactive_mode(options);
        else
        {
            char *content = get_interactive_content(false);
            status_code = execute_shell_command(options, content);
            free(content);
        }
    }
    else
        status_code = launch_script_mode(options, file_script);

    return status_code;
}

int execute_shell_command(int options, char *input)
{
    // get token_list based on given input

    if (is_option_activated(options, VERBOSE))
        puts("parsing token_list");

    struct linked_list *token_list = build_token_list(input);

    if (is_option_activated(options, VERBOSE))
        print_token_list(token_list);

    if (is_option_activated(options, VERBOSE))
        puts("building AST");

    // build AST based on token_list
    struct AST *tree = build_shell_AST(token_list);

    if (is_option_activated(options, VERBOSE))
    {
        puts("not parsed token:\n");
        print_token_list(token_list);
        puts("executing AST");
    }

    // execute tree
    int status_code = execute_AST(tree);

    deep_free_list(token_list, free_token);
    free_AST(tree);

    return status_code;
}
