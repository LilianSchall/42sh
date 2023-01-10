#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>


 // ./scipt.sh 1> test15_v.txt 2>| test15_e.txt

void redirect_15(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(exec_redirection, redirection_15, .init = redirect_15)
{
    struct linked_list *ll_ast = new_list();
    
    struct AST *ast_echo = new_AST(new_token(copy_string("tests/exec_redirection/./script.sh"), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast);
    
    struct linked_list *ll_command = new_list();
    ll_command = list_append(ll_command, ast_echo);

    struct AST *ast_seq_c = new_AST(new_token(copy_string(""), (enum token_type) COMMAND), 
                (enum AST_type) SEQUENCE, ll_command);


    struct AST *ast_fd_from = new_AST(new_token(copy_string("2"), (enum token_type) IO_NUMBER), 
    (enum AST_type) ARG, NULL);

    struct AST *ast_fd_to = new_AST(new_token(copy_string("test15_e.txt"), (enum token_type) WORD), 
    (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_redir = new_list();
    ll_redir = list_append(ll_redir, ast_fd_from);
    ll_redir = list_append(ll_redir, ast_seq_c);
    ll_redir = list_append(ll_redir, ast_fd_to);

    
    struct AST *ast_redirect = new_AST(new_token(copy_string(">|"), (enum token_type) R_SUP_PIPE), 
    (enum AST_type) REDIRECTION, ll_redir);


    struct AST *ast_fd_from2 = new_AST(new_token(copy_string("1"), (enum token_type) IO_NUMBER), 
    (enum AST_type) ARG, NULL);

    struct AST *ast_fd_to2 = new_AST(new_token(copy_string("test15_v.txt"), (enum token_type) WORD), 
    (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_redir2 = new_list();
    ll_redir2 = list_append(ll_redir2, ast_fd_from2);
    ll_redir2 = list_append(ll_redir2, ast_redirect);
    ll_redir2 = list_append(ll_redir2, ast_fd_to2);

    struct AST *ast_redirect2 = new_AST(new_token(copy_string(">"), (enum token_type) R_SUP), 
    (enum AST_type) REDIRECTION, ll_redir2);

    struct linked_list *ll_ast_2 = new_list();
    ll_ast_2 = list_append(ll_ast_2, ast_redirect2);
    
   
    struct AST *ast_final = new_AST(new_token(copy_string(""), (enum token_type) COMMAND), 
    (enum AST_type) SEQUENCE, ll_ast_2);



    execute_AST(ast_final);

    free_AST(ast_final);

    cr_assert_stdout_eq_str("");

    char * result_1 = get_file_content("test15_e.txt");

    cr_assert_str_eq(result_1, "in stderr\n");

    free(result_1);

    char * result_2 = get_file_content("test15_v.txt");

    cr_assert_str_eq(result_2, "in stdout\n");

    free(result_2);
}