#include "builtin/builtin.h"

int unset_fn(int argc, char *argv[], struct linked_list *functions)
{
    if (argc < 2)
        return 0;
    if (strcmp(argv[1], "-v") == 0)
    {
        for (int i = 2; i < argc; i++)
        {
            unsetenv(argv[i]);
        }
    }
    else if (strcmp(argv[1], "-f") == 0)
    {
        for (int i = 2; i < argc; i++)
        {
            remove_function(functions, argv[i]);
        }
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            unsetenv(argv[i]);
        }
    }
    return 0;
}
