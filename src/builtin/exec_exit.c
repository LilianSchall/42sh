#include "builtin/builtin.h"

int exit_fn(int argc, char **argv)
{
    // to many arguments => error
    if(argc > 2)
    {
        fprintf(stderr,"42sh: exit: too many arguments\n");
        return 2;
    }

    // set the exit_bool to true in order to leave execution
    status->exit_bool = 1;

    // if [n] is specified, we set it
    if(argv[1] != NULL)
        status->exit_code = my_itoa(argv[1]);
    
    return status->exit_code;
}