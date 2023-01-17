#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

#include "symbol/symbol.h"
#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

int exec3(void)
{
    struct AST *ast_1 = new_AST(new_token(new_unique_symbols(copy_string("-e"), false, false, false), (enum token_type) WORD), (enum AST_type) ARG, NULL);   
    struct AST *ast_2 = new_AST(new_token(new_unique_symbols(copy_string("test \\n test"), false, false, false), (enum token_type) WORD), (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_1);
    ll_ast = list_append(ll_ast, ast_2);
    
    struct AST *ast = new_AST(new_token(new_unique_symbols(copy_string("echo"), false, false, false), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast);
    struct linked_list *ll_ast2 = new_list();
    ll_ast2 = list_append(ll_ast2, ast);

    struct AST *ast_final = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) WORD), (enum AST_type) SEQUENCE, ll_ast2);

    execute_AST(ast_final);

    free_AST(ast_final);

    return 0;
}

void redirect_stdout3(void)
{
    cr_redirect_stdout();
}

Test(exec_echo, basic_opt_e, .init = redirect_stdout3)
{
    exec3();
    fflush(stdout);

    cr_assert_stdout_eq_str("test \n test\n");
}
