#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

static struct AST * echo(char *str)
{
    struct AST *ast_hello = new_AST(new_token(copy_string(str), (enum token_type) ARG, false), (enum AST_type) ARG, NULL);   

    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_hello);
    
    struct AST *ast_foo_1 = new_AST(new_token(copy_string("echo"), (enum token_type) WORD, false), (enum AST_type) COMMAND, ll_ast);
    
    return ast_foo_1;
}

static struct AST * get_arg(char *str)
{
    struct AST *ast_hello = new_AST(new_token(copy_string(str), (enum token_type) ARG, false), (enum AST_type) ARG, NULL);   

    return ast_hello;
}

int main()
{
    struct AST *is = echo("is");
    struct AST *equal = echo("=");

    struct linked_list *ll_seq = new_list();
    ll_seq = list_append(ll_seq, is);
    ll_seq = list_append(ll_seq, equal);

    struct AST *ast_seq = new_AST(new_token(copy_string(""), (enum token_type) ARG, false), 
    (enum AST_type) SEQUENCE, ll_seq);

    struct linked_list *ll_sub = new_list();
    ll_sub = list_append(ll_sub, ast_seq);


    struct AST *ast_sub = new_AST(new_token(copy_string(""), (enum token_type) ARG, false), 
    (enum AST_type) D_SUBSHELL, ll_sub);


    struct AST *result = get_arg("result");
    struct AST *that = get_arg("that !");


    struct linked_list *ll_echo = new_list();
    ll_echo = list_append(ll_echo, result);
    ll_echo = list_append(ll_echo, ast_sub);
    ll_echo = list_append(ll_echo, that);
    
    struct AST *ast_seq_f = new_AST(new_token(copy_string("echo"), (enum token_type) WORD, false), 
        (enum AST_type) COMMAND, ll_echo);

    struct linked_list *ll_f = new_list();
    ll_f = list_append(ll_f, ast_seq_f);
   
    struct AST *ast_final = new_AST(new_token(copy_string(""), (enum token_type) COMMAND, false), 
    (enum AST_type) SEQUENCE, ll_f);



    execute_AST(ast_final);
    free_AST(ast_final);

    return 0;
}
