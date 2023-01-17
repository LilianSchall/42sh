#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

#include "symbol/symbol.h"
#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

// cat < REAME.md > text08.txt

void redirect_8(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(exec_redirection, redirection_8, .init = redirect_8)
{
    struct linked_list *ll_ast = new_list();
    
    struct AST *ast_echo = new_AST(new_token(new_unique_symbols(copy_string("cat"), false, false, false), (enum token_type) ARG), (enum AST_type) COMMAND, ll_ast);
    
    struct linked_list *ll_command = new_list();
    ll_command = list_append(ll_command, ast_echo);

    struct AST *ast_seq_c = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) ARG), 
                (enum AST_type) SEQUENCE, ll_command);


    struct AST *ast_fd_from = new_AST(new_token(new_unique_symbols(copy_string("1"), false, false, false), (enum token_type) IO_NUMBER), 
    (enum AST_type) ARG, NULL);

    struct AST *ast_fd_to = new_AST(new_token(new_unique_symbols(copy_string("text08.txt"), false, false, false), (enum token_type) WORD), 
    (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_redir = new_list();
    ll_redir = list_append(ll_redir, ast_fd_from);
    ll_redir = list_append(ll_redir, ast_seq_c);
    ll_redir = list_append(ll_redir, ast_fd_to);

    
    struct AST *ast_redirect = new_AST(new_token(new_unique_symbols(copy_string(">"), false, false, false), (enum token_type) R_SUP), 
    (enum AST_type) REDIRECTION, ll_redir);


    struct AST *ast_fd_from2 = new_AST(new_token(new_unique_symbols(copy_string("0"), false, false, false), (enum token_type) IO_NUMBER), 
    (enum AST_type) ARG, NULL);

    struct AST *ast_fd_to2 = new_AST(new_token(new_unique_symbols(copy_string("README.md"), false, false, false), (enum token_type) WORD), 
    (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_redir2 = new_list();
    ll_redir2 = list_append(ll_redir2, ast_fd_from2);
    ll_redir2 = list_append(ll_redir2, ast_redirect);
    ll_redir2 = list_append(ll_redir2, ast_fd_to2);

    struct AST *ast_redirect2 = new_AST(new_token(new_unique_symbols(copy_string("<"), false, false, false), (enum token_type) R_INF), 
    (enum AST_type) REDIRECTION, ll_redir2);

    struct linked_list *ll_ast_2 = new_list();
    ll_ast_2 = list_append(ll_ast_2, ast_redirect2);
    
   
    struct AST *ast_final = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) ARG), 
    (enum AST_type) SEQUENCE, ll_ast_2);



    execute_AST(ast_final);

    free_AST(ast_final);

     
    
    char * result_1 = get_file_content("text08.txt");

    cr_assert_str_eq(result_1, "# 42sh");

    free(result_1);
}
