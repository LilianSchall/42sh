#include "builtin/builtin.h"

int exec_break_continue(int argc, char **argv, int *val, int nb_loop)
{
    if(argc > 0 && argv[1] == NULL)
        *val = 1;
    else
    {
        if(argv[1][0] == '-')
        {
            fprintf(stderr,"42sh: continue/break: %s: loop count out of range\n", argv[1]);
            *val = nb_loop;
            return 2;
        }

        *val = my_itoa(argv[1]);
        if (*val > nb_loop)
            *val = nb_loop;
    }
    return 0;
}
