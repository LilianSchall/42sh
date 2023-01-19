#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "symbol/symbol.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

extern struct AST * new_AST_COMMAND(char *command, char *val1, char *val2);

int exec_bool_f(void)
{

    struct AST *ast_final = new_AST_COMMAND("false", NULL, NULL);

    int result = execute_AST(ast_final);

    free_AST(ast_final);

    return result;
}

Test(exec_bool, basic_false)
{
    cr_assert_eq(exec_bool_f(), 1);
}
