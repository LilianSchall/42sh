#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

#include "symbol/symbol.h"
#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>


int exec_var(void)
{
    struct AST *ast_var_name = new_AST(new_token(new_unique_symbols(copy_string("test"), false), (enum token_type) WORD), (enum AST_type) ARG, NULL);   

    struct AST *ast_var_value = new_AST(new_token(new_unique_symbols(copy_string("prout"), false), (enum token_type) WORD), (enum AST_type) ARG, NULL);

    struct linked_list *ll_ast_ass = new_list();
    ll_ast_ass = list_append(ll_ast_ass, ast_var_name);
    ll_ast_ass = list_append(ll_ast_ass, ast_var_value);
    
    struct AST *ast_ass = new_AST(NULL, (enum AST_type) ASSIGNMENT, ll_ast_ass);
    
    struct linked_list *ll_ast_seq = new_list();
    ll_ast_seq = list_append(ll_ast_seq, ast_ass);

    struct AST *ast_seq = new_AST(NULL, (enum AST_type) SEQUENCE, ll_ast_seq);

    execute_AST(ast_seq);

    cr_expect_str_eq(getenv("test"), "prout");
    
    free_AST(ast_seq);
    
    return 0;
}

Test(exec_var, var_ast_one_var)
{
    exec_var();
}
