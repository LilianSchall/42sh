#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

int exec_bool(void)
{

    
    struct AST *ast = new_AST(new_token(copy_string("true"), (enum token_type) COMMAND), (enum AST_type) COMMAND, NULL);
    

    struct linked_list *ll_ast2 = new_list();
    ll_ast2 = list_append(ll_ast2, ast);
    
    struct AST *ast_final = new_AST(new_token(copy_string(""), (enum token_type) WORD), (enum AST_type) SEQUENCE, ll_ast2);

    int result = execute_AST(ast_final);

    free_AST(ast_final);

    return result;
}

Test(exec_bool, basic_true)
{
    cr_assert_eq(exec_bool(), 0);
}