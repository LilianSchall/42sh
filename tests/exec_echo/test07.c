#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

int exec7(void)
{
    struct AST *ast_1 = new_AST(new_token(copy_string("README.md"), (enum token_type) WORD), (enum AST_type) ARG, NULL);   
    
    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_1);
    
    struct AST *ast = new_AST(new_token(copy_string("cat"), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast);
    struct linked_list *ll_ast2 = new_list();
    ll_ast2 = list_append(ll_ast2, ast);

    struct AST *ast_final = new_AST(new_token(copy_string(""), (enum token_type) WORD), (enum AST_type) SEQUENCE, ll_ast2);

    execute_AST(ast_final);

    free_AST(ast_final);

    return 0;
}

void redirect_stdout7(void)
{
    cr_redirect_stdout();
}

Test(exec_echo, basic_cat, .init = redirect_stdout7)
{
    exec7();
    fflush(stdout);

    cr_assert_stdout_eq_str("# 42sh");
}