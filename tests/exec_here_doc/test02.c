#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "io_backend/io_backend.h"

#include "symbol/symbol.h"
#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

extern struct AST * new_AST_COMMAND(char *command, char *val1, char *val2);
struct AST *new_ITER_COMMAND(char *command, char *val1, char *val2);

void redirect(void);


Test(exec_here_doc, here_doc_2, .init = redirect)
{
    /*
    sort <<- EOF
    	5
    4
    		1
    2
    EOF
    */

    struct AST *ast_cat = new_AST_COMMAND("sort", NULL, NULL);


    struct AST *ast_io = new_AST(new_token(new_unique_symbols(copy_string("0"), false, false, false), (enum token_type) IO_NUMBER), 
    (enum AST_type) ARG, NULL);

    struct AST *ast_iter = new_ITER_COMMAND("		5\n4\n	1\n2\n", NULL, NULL);
    
    struct linked_list *ll_here = new_list();
    ll_here = list_append(ll_here, ast_io);
    ll_here = list_append(ll_here, ast_cat);
    ll_here = list_append(ll_here, ast_iter);

    
    struct AST *ast_here = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) INF_INF_MIN), 
    (enum AST_type) HERE_DOC, ll_here);


    struct linked_list *ll_ast_2 = new_list();
    ll_ast_2 = list_append(ll_ast_2, ast_here);
    
   
    struct AST *ast_final = new_AST(new_token(new_unique_symbols(copy_string(""), false, false, false), (enum token_type) ARG), 
    (enum AST_type) SEQUENCE, ll_ast_2);



    execute_AST(ast_final, NULL, NULL);
    free_AST(ast_final);

    cr_assert_stdout_eq_str("1\n2\n4\n5\n");
}
