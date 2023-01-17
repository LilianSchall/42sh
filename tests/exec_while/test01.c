#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

#include "symbol/symbol.h"
#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>


int exec_while_1(void)
{
    struct AST *ast_hello = new_AST(new_token(new_unique_symbols(copy_string("it is working !"), false, false, false), (enum token_type) ARG), (enum AST_type) ARG, NULL);   

    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_hello);
    
    struct AST *ast_echo = new_AST(new_token(new_unique_symbols(copy_string("echo"), false, false, false), (enum token_type) ARG), (enum AST_type) COMMAND, ll_ast);
    

    struct linked_list *ll_ast2 = new_list();
    ll_ast2 = list_append(ll_ast2, ast_echo);
    
    struct AST *ast_bloc = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) ARG), (enum AST_type) SEQUENCE, ll_ast2);

    // --- 


    struct AST *ast_true = new_AST(new_token(new_unique_symbols(copy_string("false"), false, false, false), (enum token_type) ARG), (enum AST_type) COMMAND, NULL);
    
    struct linked_list *ll_ast_cond = new_list();
    ll_ast_cond = list_append(ll_ast_cond, ast_true);
    
    struct AST *ast_cond = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) ARG), (enum AST_type) SEQUENCE, ll_ast_cond);

    
    struct linked_list *ll_while = new_list();
    ll_while = list_append(ll_while, ast_cond);
    ll_while = list_append(ll_while, ast_bloc);

    struct AST *ast_while = new_AST(new_token(new_unique_symbols(copy_string("while"), false, false, false), (enum token_type) WHILE), (enum AST_type) CONDITION, ll_while);

    struct linked_list *ll_seq = new_list();
    ll_seq = list_append(ll_seq, ast_while);

    struct AST *ast_seq = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) COMMAND), (enum AST_type) SEQUENCE, ll_seq);



    execute_AST(ast_seq);

    free_AST(ast_seq);

    return 0;
}


void redirect_stdout_1(void)
{
    cr_redirect_stdout();
}

Test(exec_while, while_false, .init = redirect_stdout_1)
{
    exec_while_1();
    fflush(stdout);
}
