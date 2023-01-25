#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>

void redirect(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

struct AST *new_AST_COMMAND(char *command, char *val1, char *val2)
{
    struct AST *ast_command ;
    struct AST *ast_arg_1 ;
    struct AST *ast_arg_2 ;

    ast_command = new_AST(new_token(new_unique_symbols(strdup(command), false, false, false), (enum token_type) WORD), (enum AST_type) ARG, NULL);   
    if(val1 != NULL)
        ast_arg_1 = new_AST(new_token(new_unique_symbols(copy_string(val1), false, false, false), (enum token_type) WORD), (enum AST_type) ARG, NULL);
    if(val2 != NULL)
        ast_arg_2 = new_AST(new_token(new_unique_symbols(copy_string(val2), false, false, false), (enum token_type) WORD), (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_command);
    if(val1 != NULL)
        ll_ast = list_append(ll_ast, ast_arg_1);
    if(val2 != NULL)
        ll_ast = list_append(ll_ast, ast_arg_2);        
    
    struct AST *ast = new_AST(new_token(new_unique_symbols(strdup(""), false, false, false), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast);
    
    struct linked_list *ll_ast2 = new_list();
    ll_ast2 = list_append(ll_ast2, ast);

    struct AST *ast_f = new_AST(new_token(new_unique_symbols(strdup(""), false, false, false), (enum token_type) WORD), (enum AST_type) SEQUENCE, ll_ast2);

    return ast_f;
}

struct AST *new_ITER_COMMAND(char *command, char *val1, char *val2)
{
    struct AST *ast_command ;
    struct AST *ast_arg_1 ;
    struct AST *ast_arg_2 ;

    if(command != NULL)
        ast_command = new_AST(new_token(new_unique_symbols(strdup(command), false, false, false), (enum token_type) WORD), (enum AST_type) ARG, NULL);   
    if(val1 != NULL)
        ast_arg_1 = new_AST(new_token(new_unique_symbols(copy_string(val1), false, false, false), (enum token_type) WORD), (enum AST_type) ARG, NULL);
    if(val2 != NULL)
        ast_arg_2 = new_AST(new_token(new_unique_symbols(copy_string(val2), false, false, false), (enum token_type) WORD), (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_ast = new_list();
    if(command != NULL)
        ll_ast = list_append(ll_ast, ast_command);
    if(val1 != NULL)
        ll_ast = list_append(ll_ast, ast_arg_1);
    if(val2 != NULL)
        ll_ast = list_append(ll_ast, ast_arg_2);        


    struct AST *ast_f = new_AST(new_token(new_unique_symbols(strdup(""), false, false, false), (enum token_type) WORD), (enum AST_type) ITER, ll_ast);

    return ast_f;
}
