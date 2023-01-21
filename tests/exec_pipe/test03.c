#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

#include "symbol/symbol.h"
#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

extern struct AST * new_AST_COMMAND(char *command, char *val1, char *val2);

Test(exec_redirection, redir_pipe_1)
{
    // echo blabla | cat -e > file11.txt

    struct AST *ast_seq_echo = new_AST_COMMAND("echo", "blabla", NULL);

    struct AST *ast_seq_cat = new_AST_COMMAND("cat", "-e", NULL);
    
    struct linked_list *ll_redir2 = new_list();
    ll_redir2 = list_append(ll_redir2, ast_seq_echo);
    ll_redir2 = list_append(ll_redir2, ast_seq_cat);

    
    struct AST *ast_redirect = new_AST(new_token(new_unique_symbols(copy_string("|"), false, false, false), (enum token_type) R_PIPE), 
    (enum AST_type) PIPE, ll_redir2);


    struct AST *ast_fd_from = new_AST(new_token(new_unique_symbols(copy_string("1"), false, false, false), (enum token_type) IO_NUMBER), 
        (enum AST_type) ARG, NULL);

    struct AST *ast_fd_to = new_AST(new_token(new_unique_symbols(copy_string("file11.txt"), false, false, false), (enum token_type) WORD), 
        (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_redir = new_list();
    ll_redir = list_append(ll_redir, ast_fd_from);
    ll_redir = list_append(ll_redir, ast_redirect);
    ll_redir = list_append(ll_redir, ast_fd_to);

    
    struct AST *ast_redirect2 = new_AST(new_token(new_unique_symbols(copy_string(">"), false, false, false), (enum token_type) R_SUP), 
    (enum AST_type) REDIRECTION, ll_redir);


    struct linked_list *ll_ast_2 = new_list();
    ll_ast_2 = list_append(ll_ast_2, ast_redirect2);
    
   
    struct AST *ast_final = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) COMMAND), 
        (enum AST_type) SEQUENCE, ll_ast_2);


    execute_AST(ast_final, NULL, NULL);
    free_AST(ast_final);
    
    char * result_1 = get_file_content("file11.txt");

    cr_assert_str_eq(result_1, "blabla$\n");

    free(result_1);
}
