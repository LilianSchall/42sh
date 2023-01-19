#include <err.h>
#include <stdio.h>

#include "kernel/kernel.h"
#include "option_parser/option_parser.h"

int main(int argc, char **argv)
{
    char *filename = NULL;
    char *input = NULL;
    int options = parse_given_args(argc, argv, &filename, &input);

    if (options == -1
        || (input && !is_option_activated(options, INPUT_READING)))
        errx(1, "Usage: ./42sh [options] [script] [args]");

    if (!filename)
        return launch_shell(options, filename, input, NULL);

    // search for the first token where it is equal to filename
    int begin = 0;

    while (strcmp(argv[begin], filename))
        begin++;

    int end = begin;

    while (argv[end])
        end++;

    char **new_argv = NULL;

    if (end != begin)
    {
        new_argv = calloc(end - begin + 1, sizeof(char *));
        int j = 0;
        for (int i = begin; i < end; i++)
            new_argv[j++] = argv[i];
    }

    printf("printing args\n");
    for (int i = 0; new_argv[i]; i++)
        printf("%s\n", new_argv[i]);

    int status_code = launch_shell(options, filename, input, argv);
    
    if (new_argv)
        free(new_argv);
    
    return status_code;
}

