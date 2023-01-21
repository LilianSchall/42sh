#include "builtin/builtin.h"

int exec_export(int argc, char *argv[])
{
    if (argc < 2)
       return 0;

    for (int i = 1; i < argc; i++)
    {
        char *var = strtok(argv[i], "=");
	    char *value = strtok(NULL, "=");
        if (value == NULL)
        {
	        if (setenv(var, "", 1) != 0)
            {
    	        fprintf(stderr, "42sh: export failed");
		        return 1;
		    }
	    }
	    else 
        {
            if (setenv(var, value, 1) != 0)
	        {
		        fprintf(stderr, "42sh: export failed");
                return 1;
	        }
        }
    }
    return 0;
}
