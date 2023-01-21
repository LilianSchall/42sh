#include "builtin/builtin.h" 

int cd_fn(int argc, char *argv[])
{
    char *old_cwd = getenv("OLD_PWD");
    char *home = getenv("HOME");
    char cwd[1024];
    char new_cwd[1024];
    char *dir_to_go = argv[1];
    getcwd(cwd, 1024);
    if (argc == 1)
    {
	    dir_to_go = home;
    }
    else if (argc == 2)
    {
         if(strcmp(argv[1], "-") == 0)
	 {
	    if (old_cwd == NULL)
	    {
	        fprintf(stderr, "42sh: No previous directory\n");
		return 1;
	    }
	    else
            {
	        dir_to_go = old_cwd;
	        printf("%s\n", old_cwd);
	    }
	 }
    } 
    else
    {
       fprintf(stderr, "42sh: syntax error\n");
       return 1;
    }
    if (chdir(dir_to_go) != 0)
    {
        fprintf(stderr,"42sh: cd failed\n");
        return 1;
    }
    getcwd(new_cwd, 1024);
    setenv("OLD_PWD", cwd, 1);
    setenv("PWD", new_cwd, 1);
    return 0;
}
