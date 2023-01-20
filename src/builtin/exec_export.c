#include "builtin/builtin.h"

int exec_export(int argc, char *argv[], char **envp)
{
   if (argc >= 2)
   {
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
   }
   else
   {
      for (char **env = envp; *env != 0; env++)
      {
         char *varenv = *env;
	 printf("export %s\n", varenv);
      }
   
   }
   return 0;
}
