#include "builtin/builtin.h"

int exec_unset(int argc, char *argv[])
{
    if (argc >= 2) 
    {
        if (strcmp(argv[1], "-v") == 0)
        {
	        for (int i = 2; i < argc; i++)
	        {
	            if (unsetenv(argv[i]))
	        }
	    }
	    else if (strcmp(argv[1], "-f") == 0)
        {
            for (int p = 2; p < argc; p++)
                remove_variable(NULL, argv[p]);
        }	 
        else
	    {
	        for (int j = 1; j < argc; j++)
	        {
		        unsetenv(argv[i]);
            }
        }
    }
    else
    {
        fprintf(stderr, "unset: not enough arguments");
        return 1;
    }
    return 0;
}
