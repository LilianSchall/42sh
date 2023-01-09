#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>


int exec_redirection_4(void)
{
    struct AST *ast_hello = new_AST(new_token(copy_string("foo"), (enum token_type) ARG), (enum AST_type) ARG, NULL);   

    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_hello);
    
    struct AST *ast_echo = new_AST(new_token(copy_string("echo"), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast);
    

    struct AST *ast_file = new_AST(new_token(copy_string("append.txt"), (enum token_type) WORD), (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_ast2 = new_list();
    ll_ast2 = list_append(ll_ast2, ast_echo);
    ll_ast2 = list_append(ll_ast2, ast_file);

    
    struct AST *ast_redirect = new_AST(new_token(copy_string(">>"), (enum token_type) WORD), (enum AST_type) REDIRECTION, ll_ast2);

    struct linked_list *ll_seq = new_list();
    ll_seq = list_append(ll_seq, ast_redirect);

    struct AST *ast_seq = new_AST(new_token(copy_string(""), (enum token_type) COMMAND), (enum AST_type) SEQUENCE, ll_seq);



    execute_AST(ast_seq);

    free_AST(ast_seq);

    return 0;
}

void redirect_sup(void)
{
    cr_redirect_stdout();
}

Test(exec_redirection, redirection_sup_sup, .init = redirect_sup)
{

    int file_fd = open("append.txt", O_CREAT | O_TRUNC | O_WRONLY, 0755);
    close(file_fd);
    exec_redirection_4();
    fflush(stdout);
    exec_redirection_4();
    fflush(stdout);

    char * result_2 = get_file_content("append.txt");

    cr_assert_str_eq(result_2, "foo\nfoo\n");

    cr_assert_stdout_eq_str("");
}