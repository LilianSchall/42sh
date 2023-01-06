#include <err.h>
#include <stdio.h>

#include "kernel/kernel.h"
#include "option_parser/option_parser.h"

int main(int argc, char **argv)
{
#if 0
    char *filename = NULL;
    int options = parse_given_args(argc, argv, &filename);

    if (options == -1 || 
        (filename && 
        !is_option_activated(options, SCRIPT_READING)))
        errx(1, "Usage: ./42sh [options] [script]");
    
    return launch_shell(options, filename);
}
