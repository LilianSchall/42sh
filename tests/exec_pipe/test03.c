#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

#include "symbol/symbol.h"
#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

Test(exec_redirection, redir_pipe_1)
{
    // echo blabla | cat -e > file11.txt

    struct AST *ast_hello = new_AST(new_token(new_unique_symbols(copy_string("blabla"), false), (enum token_type) ARG), (enum AST_type) ARG, NULL);   

    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_hello);
    
    struct AST *ast_echo = new_AST(new_token(new_unique_symbols(copy_string("echo"), false), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast);
    
    struct linked_list *ll_command = new_list();
    ll_command = list_append(ll_command, ast_echo);

    struct AST *ast_seq_echo = new_AST(new_token(new_unique_symbols(copy_string(""), false), (enum token_type) COMMAND), 
                (enum AST_type) SEQUENCE, ll_command);

    // cat -e 
    struct AST *ast_e = new_AST(new_token(new_unique_symbols(copy_string("-e"), false), (enum token_type) ARG), 
    (enum AST_type) ARG, NULL);   

    struct linked_list *ll_ast3 = new_list();
    ll_ast3 = list_append(ll_ast3, ast_e);
    
    struct AST *ast_cat = new_AST(new_token(new_unique_symbols(copy_string("cat"), false), (enum token_type) COMMAND), 
            (enum AST_type) COMMAND, ll_ast3);
    
    struct linked_list *ll_command3 = new_list();
    ll_command3 = list_append(ll_command3, ast_cat);

    struct AST *ast_seq_cat = new_AST(new_token(new_unique_symbols(copy_string(""), false), (enum token_type) COMMAND), 
                (enum AST_type) SEQUENCE, ll_command3);

    
    struct linked_list *ll_redir2 = new_list();
    ll_redir2 = list_append(ll_redir2, ast_seq_echo);
    ll_redir2 = list_append(ll_redir2, ast_seq_cat);

    
    struct AST *ast_redirect = new_AST(new_token(new_unique_symbols(copy_string("|"), false), (enum token_type) R_PIPE), 
    (enum AST_type) PIPE, ll_redir2);


    struct AST *ast_fd_from = new_AST(new_token(new_unique_symbols(copy_string("1"), false), (enum token_type) IO_NUMBER), 
        (enum AST_type) ARG, NULL);

    struct AST *ast_fd_to = new_AST(new_token(new_unique_symbols(copy_string("file11.txt"), false), (enum token_type) WORD), 
        (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_redir = new_list();
    ll_redir = list_append(ll_redir, ast_fd_from);
    ll_redir = list_append(ll_redir, ast_redirect);
    ll_redir = list_append(ll_redir, ast_fd_to);

    
    struct AST *ast_redirect2 = new_AST(new_token(new_unique_symbols(copy_string(">"), false), (enum token_type) R_SUP), 
    (enum AST_type) REDIRECTION, ll_redir);


    struct linked_list *ll_ast_2 = new_list();
    ll_ast_2 = list_append(ll_ast_2, ast_redirect2);
    
   
    struct AST *ast_final = new_AST(new_token(new_unique_symbols(copy_string(""), false), (enum token_type) COMMAND), 
        (enum AST_type) SEQUENCE, ll_ast_2);


    execute_AST(ast_final);
    free_AST(ast_final);
    
    char * result_1 = get_file_content("file11.txt");

    cr_assert_str_eq(result_1, "blabla$\n");

    free(result_1);
}
