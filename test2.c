#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"
#include "commons/commons.h"

#include <stdio.h>

char **split_string(char *str);

int main()
{
    char *str = strdup("Hello this\nis \"a string\"");
    char **argv = split_string(str);
    printf("%p\n", argv); 
    int i = 0;
    while (argv[i])
    {
        printf("%s\n", argv[i]);
        i++;
    }
    free(str);
}
