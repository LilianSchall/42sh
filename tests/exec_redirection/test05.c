#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

// ls ezzzz 2>& test51.txt

void redirect_5(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(exec_redirection, redirection_5, .init = redirect_5)
{
    
    struct AST *ast_hello = new_AST(new_token(copy_string("boo"), (enum token_type) ARG, false), (enum AST_type) ARG, NULL);   

    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_hello);
    
    struct AST *ast_echo = new_AST(new_token(copy_string("ls"), (enum token_type) WORD, false), (enum AST_type) COMMAND, ll_ast);
    
    struct linked_list *ll_command = new_list();
    ll_command = list_append(ll_command, ast_echo);

    struct AST *ast_seq_c = new_AST(new_token(copy_string(""), (enum token_type) COMMAND, false), 
                (enum AST_type) SEQUENCE, ll_command);


    struct AST *ast_fd_from = new_AST(new_token(copy_string("2"), (enum token_type) IO_NUMBER, false), 
    (enum AST_type) ARG, NULL);

    struct AST *ast_fd_to = new_AST(new_token(copy_string("test51.txt"), (enum token_type) WORD, false), 
    (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_redir = new_list();
    ll_redir = list_append(ll_redir, ast_fd_from);
    ll_redir = list_append(ll_redir, ast_seq_c);
    ll_redir = list_append(ll_redir, ast_fd_to);

    
    struct AST *ast_redirect = new_AST(new_token(copy_string(">&"), (enum token_type) R_SUP_AND, false), 
    (enum AST_type) REDIRECTION, ll_redir);


    struct linked_list *ll_ast_2 = new_list();
    ll_ast_2 = list_append(ll_ast_2, ast_redirect);
    
   
    struct AST *ast_final = new_AST(new_token(copy_string(""), (enum token_type) COMMAND, false), 
    (enum AST_type) SEQUENCE, ll_ast_2);



    execute_AST(ast_final);
    free_AST(ast_final);

     
     

    char * result_1 = get_file_content("test51.txt");

    cr_assert_str_eq(result_1, "ls: cannot access 'boo': No such file or directory\n");

    free(result_1);
}

