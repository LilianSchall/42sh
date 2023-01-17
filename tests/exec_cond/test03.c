#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "symbol/symbol.h"
int exec_cond_3(void)
{
    struct AST *ast_false = new_AST(new_token(new_unique_symbols(copy_string("false"), false, false, false), (enum token_type) WORD), (enum AST_type) COMMAND, NULL);   

    struct linked_list *ll_false = new_list();
    ll_false = list_append(ll_false, ast_false);
    
    struct AST *ast_seq_1 = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) WORD), (enum AST_type) SEQUENCE, ll_false);

    
    struct AST *ast_true = new_AST(new_token(new_unique_symbols(copy_string("true"), false, false, false), (enum token_type) WORD), (enum AST_type) COMMAND, NULL);   

    struct linked_list *ll_true = new_list();
    ll_true = list_append(ll_true, ast_true);
    
    struct AST *ast_seq_2 = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) WORD), (enum AST_type) SEQUENCE, ll_true);


    struct linked_list *ll_cond = new_list();
    ll_cond = list_append(ll_cond, ast_seq_1);
    ll_cond = list_append(ll_cond, ast_seq_2);

    struct AST *ast_cond = new_AST(new_token(new_unique_symbols(copy_string("&&"), false, false, false), (enum token_type) AND), (enum AST_type) OPERATOR, ll_cond);
    
    struct linked_list *ll_seq = new_list();
    ll_seq = list_append(ll_seq, ast_cond);
    
    struct AST *ast = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) WORD), (enum AST_type) SEQUENCE, ll_seq);

    int val = execute_AST(ast);

    free_AST(ast);

    return val;
}


Test(exec_cond, cond_and_1_0)
{
    cr_assert_eq(exec_cond_3(), 1);
}
