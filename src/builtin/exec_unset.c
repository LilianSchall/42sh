#include "builtin/builtin.h"
int exec_unset(int argc, char *argv[])
{
    if (argc >= 2) 
    {
         if (strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "-v") == 0 )
         {
	     for (int i = 2; i < argc; i++)
	     {
	         if (unsetenv(argv[i]) != 0)
	         {
		     fprintf(stderr, "Invalid argument: %s\n", argv[i]);
		     return 1;
		 }
	     }
	 }
	 else
	 {
	     for (int j = 1; j < argc; j++)
	     {
		 if (unsetenv(argv[j]) != 0)
	         {
	             fprintf(stderr, "Invalid argument: %s\n", argv[j]);
                     return 1;
		 }
	     }
	 }
    }
    else
    {
        fprintf(stderr, "Invalid usage of unset");
        return 1;
    }
    return 0;
}
