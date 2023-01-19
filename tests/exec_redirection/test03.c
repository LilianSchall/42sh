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

int exec_redirection_3(void)
{
    // echo hello >> file.txt


    struct AST *ast_seq_c = new_AST_COMMAND("echo", "hello", NULL);


    struct AST *ast_fd_from = new_AST(new_token(new_unique_symbols(copy_string("1"), false, false, false), (enum token_type) IO_NUMBER), 
    (enum AST_type) ARG, NULL);

    struct AST *ast_fd_to = new_AST(new_token(new_unique_symbols(copy_string("test31.txt"), false, false, false), (enum token_type) ARG), 
    (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_redir = new_list();
    ll_redir = list_append(ll_redir, ast_fd_from);
    ll_redir = list_append(ll_redir, ast_seq_c);
    ll_redir = list_append(ll_redir, ast_fd_to);

    
    struct AST *ast_redirect = new_AST(new_token(new_unique_symbols(copy_string(">>"), false, false, false), (enum token_type) R_SUP_SUP), 
    (enum AST_type) REDIRECTION, ll_redir);


    struct linked_list *ll_ast_2 = new_list();
    ll_ast_2 = list_append(ll_ast_2, ast_redirect);
    
   
    struct AST *ast_final = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) ARG), 
    (enum AST_type) SEQUENCE, ll_ast_2);


    execute_AST(ast_final);
    free_AST(ast_final);
    return 0;
}

void redirect_3(void)
{
    cr_redirect_stdout();
}

Test(exec_redirection, redirection_3, .init = redirect_3)
{

    int file_fd = open("append.txt", O_CREAT | O_TRUNC | O_WRONLY, 0755);
    close(file_fd);
    exec_redirection_3();
    exec_redirection_3();

    char * result_2 = get_file_content("test31.txt");

    cr_assert_str_eq(result_2, "hello\nhello\n");

     
}
