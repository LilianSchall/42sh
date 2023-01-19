#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

extern struct AST * new_AST_COMMAND(char *command, char *val1, char *val2);

static void redirect(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

static struct AST * echo_a(void)
{
    struct AST *ast_hello = new_AST(new_token(new_unique_symbols(copy_string("$a"), true, false, false), (enum token_type) WORD), (enum AST_type) ARG, NULL);   
    struct AST *ast_echo = new_AST(new_token(new_unique_symbols(copy_string("echo"), false, false, false), (enum token_type) WORD), (enum AST_type) ARG, NULL);   
    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_echo);
    ll_ast = list_append(ll_ast, ast_hello);
    struct AST *ast_foo_1 = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast);
    
    return ast_foo_1;
}

static struct AST * set_a_5(void)
{
    struct AST *ast_hello = new_AST(new_token(new_unique_symbols(copy_string("5"), false, false, false), (enum token_type) WORD), (enum AST_type) ARG, NULL);   
    
    struct AST *ast_foo_1 = new_AST(new_token(new_unique_symbols(copy_string("a"), false, false, false), (enum token_type) WORD), (enum AST_type) COMMAND, NULL);


    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_foo_1);
    ll_ast = list_append(ll_ast, ast_hello);

    struct AST *ast = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) WORD), (enum AST_type) ASSIGNMENT, ll_ast);

    return ast;
}

static struct AST * set_a_10(void)
{
    struct AST *ast_hello = new_AST(new_token(new_unique_symbols(copy_string("10"), false, false, false), (enum token_type) WORD), (enum AST_type) ARG, NULL);   
    
    struct AST *ast_foo_1 = new_AST(new_token(new_unique_symbols(copy_string("a"), false, false, false), (enum token_type) WORD), (enum AST_type) COMMAND, NULL);


    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_foo_1);
    ll_ast = list_append(ll_ast, ast_hello);

    struct AST *ast = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) WORD), (enum AST_type) ASSIGNMENT, ll_ast);

  

    return ast;
}

Test(exec_subshell, exec_subshell_1, .init = redirect)
{
    // a=5; echo $a; (echo $a; a=10; echo $a;); echo $a
    struct AST *a1 = set_a_5();
    struct AST *e1 = echo_a();


    struct AST *e2 = echo_a();
    struct AST *a2 = set_a_10();
    struct AST *e3 = echo_a();


    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, e2);
    ll_ast = list_append(ll_ast, a2);
    ll_ast = list_append(ll_ast, e3);
    

    struct AST *ast_seq = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) WORD), 
    (enum AST_type) SEQUENCE, ll_ast);

    struct linked_list *ll = new_list();
    ll = list_append(ll, ast_seq);
    

    struct AST *ast_sub = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) WORD), 
    (enum AST_type) SUBSHELL, ll);



    struct AST *e4 = echo_a();

    struct linked_list *ll_ast_2 = new_list();
    ll_ast_2 = list_append(ll_ast_2, a1);
    ll_ast_2 = list_append(ll_ast_2, e1);
    ll_ast_2 = list_append(ll_ast_2, ast_sub);
    ll_ast_2 = list_append(ll_ast_2, e4);
    

   
    struct AST *ast_final = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) WORD), 
    (enum AST_type) SEQUENCE, ll_ast_2);



    execute_AST(ast_final, NULL);
    free_AST(ast_final);


    cr_assert_stdout_eq_str("5\n5\n10\n5\n");
}
