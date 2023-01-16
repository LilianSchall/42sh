#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

#include "symbol/symbol.h"
#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>


void redirect_pipe_2(void)
{
    cr_redirect_stdout();
}

Test(exec_redirection, pipe_2, .init = redirect_pipe_2)
{
    // echo blabla | head -c 10 | cat -e 
    struct AST *ast_hello = new_AST(new_token(new_unique_symbols(copy_string("blabla"), false), (enum token_type) ARG), (enum AST_type) ARG, NULL);   

    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_hello);
    
    struct AST *ast_echo = new_AST(new_token(new_unique_symbols(copy_string("echo"), false), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast);
    
    struct linked_list *ll_command = new_list();
    ll_command = list_append(ll_command, ast_echo);

    struct AST *ast_seq_echo = new_AST(new_token(new_unique_symbols(copy_string(""), false), (enum token_type) WORD), 
                (enum AST_type) SEQUENCE, ll_command);


   // echo hello 
    struct AST *ast_c = new_AST(new_token(new_unique_symbols(copy_string("-c"), false), (enum token_type) WORD), (enum AST_type) ARG, NULL);   
    struct AST *ast_5 = new_AST(new_token(new_unique_symbols(copy_string("1"), false), (enum token_type) WORD), (enum AST_type) ARG, NULL);   

    struct linked_list *ll_ast2 = new_list();
    ll_ast2 = list_append(ll_ast2, ast_c);
    ll_ast2 = list_append(ll_ast2, ast_5);
    
    struct AST *ast_head = new_AST(new_token(new_unique_symbols(copy_string("head"), false), (enum token_type) WORD), 
        (enum AST_type) COMMAND, ll_ast2);
    
    struct linked_list *ll_comman_2d = new_list();
    ll_comman_2d = list_append(ll_comman_2d, ast_head);

    struct AST *ast_seq_head = new_AST(new_token(new_unique_symbols(copy_string(""), false), (enum token_type) WORD), 
                (enum AST_type) SEQUENCE, ll_comman_2d);



    // cat -e 
    struct AST *ast_e = new_AST(new_token(new_unique_symbols(copy_string("-e"), false), (enum token_type) WORD), 
    (enum AST_type) ARG, NULL);   

    struct linked_list *ll_ast3 = new_list();
    ll_ast3 = list_append(ll_ast3, ast_e);
    
    struct AST *ast_cat = new_AST(new_token(new_unique_symbols(copy_string("cat"), false), (enum token_type) WORD), 
            (enum AST_type) COMMAND, ll_ast3);
    
    struct linked_list *ll_command3 = new_list();
    ll_command3 = list_append(ll_command3, ast_cat);

    struct AST *ast_seq_cat = new_AST(new_token(new_unique_symbols(copy_string(""), false), (enum token_type) WORD), 
                (enum AST_type) SEQUENCE, ll_command3);

    
    struct linked_list *ll_redir = new_list();
    ll_redir = list_append(ll_redir, ast_seq_echo);
    ll_redir = list_append(ll_redir, ast_seq_head);
    ll_redir = list_append(ll_redir, ast_seq_cat);

    
    struct AST *ast_redirect = new_AST(new_token(new_unique_symbols(copy_string("|"), false), (enum token_type) R_PIPE), 
    (enum AST_type) PIPE, ll_redir);


    struct linked_list *ll_ast_2 = new_list();
    ll_ast_2 = list_append(ll_ast_2, ast_redirect);
    
   
    struct AST *ast_final = new_AST(new_token(new_unique_symbols(copy_string(""), false), (enum token_type) WORD), 
    (enum AST_type) SEQUENCE, ll_ast_2);



    execute_AST(ast_final);
    free_AST(ast_final);

    
    cr_assert_stdout_eq_str("b");

}
