#include "builtin/builtin.h"

int exec_break_continue(int argc, char **argv, int *val)
{
    if(argc > 0 && argv[1] == NULL)
        *val = 1;
    else
    {
        if(argv[1][0] == '-')
        {
            fprintf(stderr,"42sh: continue/break: %s: loop count out of range\n", argv[1]);
            *val = status->nb_loop;
            return 2;
        }

        *val = my_itoa(argv[1]);
        if (*val > status->nb_loop)
            *val = status->nb_loop;
    }
    return 0;
}
