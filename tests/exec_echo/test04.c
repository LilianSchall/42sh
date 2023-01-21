#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

#include "symbol/symbol.h"
#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

extern struct AST * new_AST_COMMAND(char *command, char *val1, char *val2);

int exec4(void)
{
    
    struct AST *ast_final = new_AST_COMMAND("echo", "-neE", "test \\n test");

    execute_AST(ast_final, NULL, NULL);

    free_AST(ast_final);
    
    return 0;
}

void redirect_stdout4(void)
{
    cr_redirect_stdout();
}

Test(exec_echo, basic_opt_eEn, .init = redirect_stdout4)
{
    exec4();
    fflush(stdout);

    cr_assert_stdout_eq_str("test \\n test");
}
