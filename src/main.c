#include <err.h>

#include "kernel/kernel.h"
#include "option_parser/option_parser.h"

int main(int argc, char **argv)
{
    char *filename = NULL;
    int options = parse_given_args(argc, argv, &filename);

    if (options == -1 || 
        (filename && 
        !is_option_activated(options, script_reading))
        errx(1, "Usage: ./42sh [options] [script]");
    
    

    return options;
}
