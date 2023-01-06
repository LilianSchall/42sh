#include "io_backend.h"

char *get_file_content(char *filename)
{
    int len = 0;
    int i = 0;
    char c;
    FILE *file = fopen(filename,"r");

    while (fgetc(file) != EOF)
    {
    len++;
    }
    char return_str[len];
    while ((c = fgetc(file)) != EOF)
    {
       return_str[i] = c;
       i++;
    }
    fclose(file);
    return return_str;
}

char *get_interactive_content(void)
{
    // TODO
    return NULL;
}
