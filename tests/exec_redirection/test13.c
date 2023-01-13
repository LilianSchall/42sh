#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

// ./scrip.sh 2>> errorlog.txt

int exec_redirection_13(void)
{
    struct linked_list *ll_ast = new_list();
    
    struct AST *ast_echo = new_AST(new_token(copy_string("tests/exec_redirection/./script.sh"), (enum token_type) WORD, false), (enum AST_type) COMMAND, ll_ast);
    
    struct linked_list *ll_command = new_list();
    ll_command = list_append(ll_command, ast_echo);

    struct AST *ast_seq_c = new_AST(new_token(copy_string(""), (enum token_type) COMMAND, false), 
                (enum AST_type) SEQUENCE, ll_command);


    struct AST *ast_fd_from = new_AST(new_token(copy_string("2"), (enum token_type) IO_NUMBER, false), 
    (enum AST_type) ARG, NULL);

    struct AST *ast_fd_to = new_AST(new_token(copy_string("test131.txt"), (enum token_type) WORD, false), 
    (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_redir = new_list();
    ll_redir = list_append(ll_redir, ast_fd_from);
    ll_redir = list_append(ll_redir, ast_seq_c);
    ll_redir = list_append(ll_redir, ast_fd_to);

    
    struct AST *ast_redirect = new_AST(new_token(copy_string(">>"), (enum token_type) R_SUP_SUP, false), 
    (enum AST_type) REDIRECTION, ll_redir);


    struct linked_list *ll_ast_2 = new_list();
    ll_ast_2 = list_append(ll_ast_2, ast_redirect);
    
   
    struct AST *ast_final = new_AST(new_token(copy_string(""), (enum token_type) COMMAND, false), 
    (enum AST_type) SEQUENCE, ll_ast_2);

    execute_AST(ast_final);
    free_AST(ast_final);

    return 0;
}

void redirect_13(void)
{
    cr_redirect_stdout();
}

Test(exec_redirection, redirection_13, .init = redirect_13)
{

    int file_fd = open("append.txt", O_CREAT | O_TRUNC | O_WRONLY, 0755);
    close(file_fd);
    exec_redirection_13();
    exec_redirection_13();

    char * result_2 = get_file_content("test131.txt");

    cr_assert_str_eq(result_2, "in stderr\nin stderr\n");

    free(result_2);
}
