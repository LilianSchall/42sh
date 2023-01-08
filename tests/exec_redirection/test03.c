#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>


Test(exec_redirection, redirection_sup_3)
{
    struct AST *ast_hello = new_AST(new_token(copy_string("it is working !"), (enum token_type) ARG), (enum AST_type) ARG, NULL);   
    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_hello);
    
    struct AST *ast_echo = new_AST(new_token(copy_string("echo"), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast);
    

    struct AST *ast_file = new_AST(new_token(copy_string("file1.txt"), (enum token_type) WORD), (enum AST_type) ARG, NULL);
    
    struct linked_list *ll_ast2 = new_list();
    ll_ast2 = list_append(ll_ast2, ast_echo);
    ll_ast2 = list_append(ll_ast2, ast_file);
    
    struct AST *ast_redirect = new_AST(new_token(copy_string(">"), (enum token_type) WORD), (enum AST_type) REDIRECTION, ll_ast2);

    
    
    struct AST *ast_file2 = new_AST(new_token(copy_string("file2.txt"), (enum token_type) WORD), (enum AST_type) ARG, NULL);
    

    struct linked_list *ll_seq3 = new_list();

    ll_seq3 = list_append(ll_seq3, ast_redirect);
    ll_seq3 = list_append(ll_seq3, ast_file2);

    struct AST *ast_redirect2 = new_AST(new_token(copy_string(">"), (enum token_type) WORD), (enum AST_type) REDIRECTION, ll_seq3);

    

    struct linked_list *ll_seq2= new_list();
    ll_seq2 = list_append(ll_seq2, ast_redirect2);


    struct AST *ast_seq = new_AST(new_token(copy_string(""), (enum token_type) COMMAND), (enum AST_type) SEQUENCE, ll_seq2);

    execute_AST(ast_seq);
    free_AST(ast_seq);

    char * result_2 = get_file_content("file2.txt");

    cr_assert_str_eq(result_2, "");

    char * result_1 = get_file_content("file1.txt");

    cr_assert_str_eq(result_1, "it is working !\n");
    free(result_1);
    free(result_2);
}
