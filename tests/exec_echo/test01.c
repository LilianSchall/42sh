#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

int exec(void)
{
    struct AST *ast_1 = new_AST(new_token(copy_string("hello"), (enum token_type) WORD), (enum AST_type) ARG, NULL);   
    struct AST *ast_2 = new_AST(new_token(copy_string("hello word!"), (enum token_type) WORD), (enum AST_type) ARG, NULL);

    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_1);
    ll_ast = list_append(ll_ast, ast_2);
    
    struct AST *ast = new_AST(new_token(copy_string("echo"), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast);

    struct linked_list *ll_word = get_linked_list_from_AST(ast);

    int argc = 0;

    char **argv = new_argv(ll_word, &argc);
    

    echo_fn(argc, argv);
    free_argv(argc, argv);
    free_list(ll_word);
    free_AST(ast);

    return 0;
}

void redirect_stdout(void)
{
    cr_redirect_stdout();
}

Test(exec_echo, exit_code, .init = redirect_stdout)
{
    exec();
    fflush(stdout);

    cr_assert_stdout_eq_str("hello world !\n");
}