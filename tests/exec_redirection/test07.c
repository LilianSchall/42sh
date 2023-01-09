#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>


int exec_redirection_7(void)
{
    struct linked_list *ll_empty = new_list();
    
    struct AST *ast_echo = new_AST(new_token(copy_string("cat"), (enum token_type) WORD), (enum AST_type) COMMAND, ll_empty);
    

    struct AST *ast_file = new_AST(new_token(copy_string("README.md"), (enum token_type) WORD), (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_ast2 = new_list();
    ll_ast2 = list_append(ll_ast2, ast_echo);
    ll_ast2 = list_append(ll_ast2, ast_file);

    
    struct AST *ast_redirect = new_AST(new_token(copy_string("<"), (enum token_type) WORD), (enum AST_type) REDIRECTION, ll_ast2);

    struct linked_list *ll_seq = new_list();
    ll_seq = list_append(ll_seq, ast_redirect);

    struct AST *ast_seq = new_AST(new_token(copy_string(""), (enum token_type) COMMAND), (enum AST_type) SEQUENCE, ll_seq);



    execute_AST(ast_seq);

    free_AST(ast_seq);

    return 0;
}

void redirect_sup_3(void)
{
    cr_redirect_stdout();
}

Test(exec_redirection, redirection_inf_basic, .init = redirect_sup_3)
{
    // cat < readme.md
    exec_redirection_7();
    fflush(stdout);

    cr_assert_stdout_eq_str("# 42sh");
}