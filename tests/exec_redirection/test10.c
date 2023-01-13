#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

void redirect_10(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(exec_redirection, redirection_if_10, .init = redirect_10)
{
    // if true; then echo it is working !; fi > test101.txt
    
    struct AST *ast_hello = new_AST(new_token(copy_string("it is working !"), (enum token_type) ARG, false), (enum AST_type) ARG, NULL);   

    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_hello);
    
    struct AST *ast_echo = new_AST(new_token(copy_string("echo"), (enum token_type) WORD, false), (enum AST_type) COMMAND, ll_ast);
    

    struct linked_list *ll_ast2 = new_list();
    ll_ast2 = list_append(ll_ast2, ast_echo);
    
    struct AST *ast_bloc = new_AST(new_token(copy_string(""), (enum token_type) WORD, false), (enum AST_type) SEQUENCE, ll_ast2);

    // --- 


    struct AST *ast_true = new_AST(new_token(copy_string("true"), (enum token_type) WORD, false), (enum AST_type) COMMAND, NULL);
    
    struct linked_list *ll_ast_cond = new_list();
    ll_ast_cond = list_append(ll_ast_cond, ast_true);
    
    struct AST *ast_cond = new_AST(new_token(copy_string(""), (enum token_type) WORD, false), (enum AST_type) SEQUENCE, ll_ast_cond);

    
    struct linked_list *ll_while = new_list();
    ll_while = list_append(ll_while, ast_cond);
    ll_while = list_append(ll_while, ast_bloc);

    struct AST *ast_if = new_AST(new_token(copy_string("if"), (enum token_type) IF, false), 
        (enum AST_type) CONDITION, ll_while);



    struct AST *ast_fd_from = new_AST(new_token(copy_string("1"), (enum token_type) IO_NUMBER, false), 
    (enum AST_type) ARG, NULL);

    struct AST *ast_fd_to = new_AST(new_token(copy_string("test101.txt"), (enum token_type) WORD, false), 
    (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_redir = new_list();
    ll_redir = list_append(ll_redir, ast_fd_from);
    ll_redir = list_append(ll_redir, ast_if);
    ll_redir = list_append(ll_redir, ast_fd_to);


    struct AST *ast_redirect = new_AST(new_token(copy_string(">"), (enum token_type) R_SUP, false), 
        (enum AST_type) REDIRECTION, ll_redir);

    struct linked_list *ll_seq = new_list();
    ll_seq = list_append(ll_seq, ast_redirect);

    struct AST *ast_seq = new_AST(new_token(copy_string(""), (enum token_type) COMMAND, false), 
        (enum AST_type) SEQUENCE, ll_seq);



    execute_AST(ast_seq);

    free_AST(ast_seq);

    char * result_1 = get_file_content("test101.txt");

    cr_assert_str_eq(result_1, "it is working !\n");

    free(result_1);
}

