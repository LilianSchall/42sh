#include "io_backend.h"
#define MAX_LENGTH 1024 
char *get_file_content(char *filename)
{
    FILE *file = fopen(filename,"r");
    char buffer[MAX_LENGTH];
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *return_str = calloc(len + 1, 1);
    while (fgets(buffer, MAX_LENGTH, file) != NULL)
    {
       strcat(return_str, buffer);
    }
    fclose(file);
    return return_str;
}

char *get_interactive_content(void)
{
    char buffer[MAX_LENGTH];
    size_t capacity = MAX_LENGTH;
    size_t size = 0;
    char *return_str = calloc(capacity, 1);
    while (fgets(buffer, MAX_LENGTH, stdin) != NULL)
    {
       size_t len = strlen(buffer);
       if (size + len + 1 >= capacity)
       {
           return_str = realloc(return_str, capacity + MAX_LENGTH);
           capacity += MAX_LENGTH;
       }
       strcat(return_str, buffer);
    }
    return return_str; 
}
