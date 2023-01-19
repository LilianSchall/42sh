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

void redirect_pipe_1(void)
{
    cr_redirect_stdout();
}

Test(exec_redirection, pipe_1, .init = redirect_pipe_1)
{
    // echo blabla | head -c 10 | cat -e 


    struct AST *ast_seq_echo = new_AST_COMMAND("echo", "blabla", NULL);

    struct AST *ast_seq_head = new_AST_COMMAND("head", "-c", "10");

    struct AST *ast_seq_cat = new_AST_COMMAND("cat", "-e", NULL);

    
    struct linked_list *ll_redir = new_list();
    ll_redir = list_append(ll_redir, ast_seq_echo);
    ll_redir = list_append(ll_redir, ast_seq_head);
    ll_redir = list_append(ll_redir, ast_seq_cat);

    
    struct AST *ast_redirect = new_AST(new_token(new_unique_symbols(copy_string("|"), false, false, false), (enum token_type) R_PIPE), 
    (enum AST_type) PIPE, ll_redir);


    struct linked_list *ll_ast_2 = new_list();
    ll_ast_2 = list_append(ll_ast_2, ast_redirect);
    
   
    struct AST *ast_final = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) COMMAND), 
    (enum AST_type) SEQUENCE, ll_ast_2);



    execute_AST(ast_final);
    free_AST(ast_final);

    
    cr_assert_stdout_eq_str("blabla$\n");

}
