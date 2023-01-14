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
        errx(1, "Usage: ./42sh [options] [script]");

    return launch_shell(options, filename, input);
}

