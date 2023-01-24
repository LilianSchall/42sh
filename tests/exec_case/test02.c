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


Test(exec_case, case_2, .init = redirect)
{
    struct AST *val = new_ITER_COMMAND("10", NULL, NULL);

    struct AST *case_1 = new_ITER_COMMAND("10", NULL, NULL);
    struct AST *seq_1 = new_AST_COMMAND("echo","case 1 : 10", NULL);

    struct AST *case_2 = new_ITER_COMMAND("1000", "100", NULL);
    struct AST *seq_2 = new_AST_COMMAND("echo","case 2 : 1000 or 100", NULL);

    struct AST *seq_3 = new_AST_COMMAND("echo","default", "case");

    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, val);
    ll_ast = list_append(ll_ast, case_1);
    ll_ast = list_append(ll_ast, seq_1);
    ll_ast = list_append(ll_ast, case_2);
    ll_ast = list_append(ll_ast, seq_2);
    ll_ast = list_append(ll_ast, seq_3);

    
    struct AST *ast_case = new_AST(new_token(new_unique_symbols(strdup(""), false, false, false), (enum token_type) WORD), (enum AST_type) CASE_C, ll_ast);

    struct linked_list *ll_ast2 = new_list();
    ll_ast2 = list_append(ll_ast2, ast_case);  

    struct AST *ast = new_AST(new_token(new_unique_symbols(strdup(""), false, false, false), (enum token_type) WORD), (enum AST_type) SEQUENCE, ll_ast2);


    execute_AST(ast, NULL, NULL);

    free_AST(ast);


    cr_assert_stdout_eq_str("case 1 : 10\n");

}
