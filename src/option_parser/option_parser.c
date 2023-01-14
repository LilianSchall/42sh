#include "option_parser.h"

int parse_given_args(int argc, char **argv, char **filename, char **input)
{
    int options = 0;
    *filename = NULL;

    // iterate over arguments
    for (int i = 1; i < argc; i++)
    {
        // check verbose mode
        if (!strcmp(argv[i], "--verbose"))
            options = options | VERBOSE;
        // check pretty print mode
        else if (!strcmp(argv[i], "--pretty-print"))
            options = options | PRETTY_PRINT;
        // check if file is specified
        else if (!strcmp(argv[i], "-c") && *input == NULL && *filename == NULL)
        {
            // if there is an argument after option
            // then we got a filename
            if (i++ < argc)
                *input = argv[i];
            else // else we crash
                return -1;
            options = options | INPUT_READING;
        }
        else if (*filename == NULL && *input == NULL)
        {
            *filename = argv[i];
            options = options | SCRIPT_READING;
        }
        else
            return -1;
    }

    return options;
}

bool is_option_activated(int options, int mode)
{
    if (options & mode)
        return true;
    return false;
}

