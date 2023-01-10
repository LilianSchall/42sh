#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>


int test12(void) // ./scrip.sh 1>& 2
{
    struct linked_list *ll_ast = new_list();
    
    struct AST *ast_echo = new_AST(new_token(copy_string("tests/exec_redirection/./script.sh"), 
                            (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast);
    

    struct AST *ast_file = new_AST(new_token(copy_string("2"), (enum token_type) WORD), 
                            (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_ast2 = new_list();
    ll_ast2 = list_append(ll_ast2, ast_echo);
    ll_ast2 = list_append(ll_ast2, ast_file);

    
    struct AST *ast_redirect = new_AST(new_token(copy_string("1>&"), (enum token_type) WORD), 
                                (enum AST_type) REDIRECTION, ll_ast2);

    struct linked_list *ll_seq = new_list();
    ll_seq = list_append(ll_seq, ast_redirect);

    struct AST *ast_seq = new_AST(new_token(copy_string(""), (enum token_type) COMMAND), 
                            (enum AST_type) SEQUENCE, ll_seq);

    execute_AST(ast_seq);

    free_AST(ast_seq);

    return 0;
}

void redirect_out_2(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(exec_redirection, redirection_12, .init = redirect_out_2)
{
    test12();

    cr_assert_stdout_eq_str("");

    cr_assert_stderr_eq_str("in stdout\nin stderr\n");
}