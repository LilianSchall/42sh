#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>


int exec_redirection_1(void)
{
    struct AST *ast_hello = new_AST(new_token(copy_string("writting in file!"), (enum token_type) ARG), (enum AST_type) ARG, NULL);   

    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_hello);
    
    struct AST *ast_echo = new_AST(new_token(copy_string("echo"), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast);
    

    struct AST *ast_file = new_AST(new_token(copy_string("testRedirect.txt"), (enum token_type) WORD), (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_ast2 = new_list();
    ll_ast2 = list_append(ll_ast2, ast_echo);
    ll_ast2 = list_append(ll_ast2, ast_file);

    
    struct AST *ast_redirect = new_AST(new_token(copy_string(">"), (enum token_type) WORD), (enum AST_type) REDIRECTION, ll_ast2);

    struct AST *ast_hello_2 = new_AST(new_token(copy_string("in stdout!"), (enum token_type) ARG), (enum AST_type) ARG, NULL);   

    struct linked_list *ll_ast_2 = new_list();
    ll_ast_2 = list_append(ll_ast_2, ast_hello_2);
    
    struct AST *ast_echo2 = new_AST(new_token(copy_string("echo"), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast_2);
    

    struct linked_list *ll_seq = new_list();
    ll_seq = list_append(ll_seq, ast_redirect);
    ll_seq = list_append(ll_seq, ast_echo2);

    struct AST *ast_seq = new_AST(new_token(copy_string(""), (enum token_type) COMMAND), (enum AST_type) SEQUENCE, ll_seq);



    execute_AST(ast_seq);

    free_AST(ast_seq);

    return 0;
}

void redirect_2(void)
{
    cr_redirect_stdout();
}

Test(exec_redirection, redirection_sup_1, .init = redirect_2)
{
    exec_redirection_1();
    fflush(stdout);

    cr_assert_stdout_eq_str("in stdout!\n");
}