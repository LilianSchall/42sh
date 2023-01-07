#include "builtin/builtin.h"

int true_fn(int argc, char **argv)
{
    if (argc || argv)
        return 0;
    else
        return 0;
}

int false_fn(int argc, char **argv)
{
    if (argc || argv)
        return 1;
    else
        return 1;
}
