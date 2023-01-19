#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"
#include "expansion/expansion.h"

#include <stdio.h>

char *get_var_star(int quoted);

int main()
{
    char* argv[] = { "1", "2", "3", NULL};
    set_spec_var(4, argv);

    printf("%s\n", get_var_star(1));

    return 0;
}
