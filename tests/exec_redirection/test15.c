#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

void redirect_15(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}


Test(exec_redirection, redirection_15, .init = redirect_15)
{
    // ./scipt.sh 1> Dvalid.txt 2> Derror.txt

    struct linked_list *ll_ast = new_list();
    
    struct AST *ast_echo = new_AST(new_token(copy_string("tests/exec_redirection/./script.sh"), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast);
    

    struct AST *ast_file = new_AST(new_token(copy_string("Derror.txt"), (enum token_type) WORD), (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_ast2 = new_list();
    ll_ast2 = list_append(ll_ast2, ast_echo);
    ll_ast2 = list_append(ll_ast2, ast_file);
    
    struct AST *ast_redirect = new_AST(new_token(copy_string("2>"), (enum token_type) WORD), (enum AST_type) REDIRECTION, ll_ast2);

    
    struct AST *ast_file2 = new_AST(new_token(copy_string("Dvalid.txt"), (enum token_type) WORD), (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_seq3 = new_list();

    ll_seq3 = list_append(ll_seq3, ast_redirect);
    ll_seq3 = list_append(ll_seq3, ast_file2);

    struct AST *ast_redirect2 = new_AST(new_token(copy_string("1>"), (enum token_type) WORD), (enum AST_type) REDIRECTION, ll_seq3);

    

    struct linked_list *ll_seq2= new_list();
    ll_seq2 = list_append(ll_seq2, ast_redirect2);


    struct AST *ast_seq = new_AST(new_token(copy_string(""), (enum token_type) COMMAND), (enum AST_type) SEQUENCE, ll_seq2);

    execute_AST(ast_seq);
    free_AST(ast_seq);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");

    char *result_2 = get_file_content("Dvalid.txt");

    cr_assert_str_eq(result_2, "in stdout\n");

    char *result_3 = get_file_content("Derror.txt");

    cr_assert_str_eq(result_3, "in stderr\n");
}
