#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "AST/AST.h"
#include "builtin/exec_echo.h"
#include "linked_list/linked_list.h"
#include "token/token.h"


void print_list(struct linked_list *ast_list)
{
    printf("-----");

    while(ast_list->head != NULL)
    {
        char *s = list_head(ast_list);
        printf("\n%s", s);
        ast_list = list_pop(ast_list);
    }
    printf("\n----\n");

}


char *copy_string2(char *src)
{
    int l = strlen(src);
    char *dest = malloc(sizeof(char) * (l + 1));
    return strcpy(dest, src);
}

int exec(void)
{
    struct AST *ast_word = new_AST(new_token(copy_string2("test \\n test"), (enum token_type) WORD), (enum AST_type) ARG, NULL);

    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_word);
    
    struct AST *ast = new_AST(new_token(copy_string2("-Ene"), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast);

    struct linked_list *ll_ast2 = new_list();
    ll_ast2 = list_append(ll_ast2, ast);
    
    struct AST *ast2 = new_AST(new_token(copy_string2("echo"), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast2);

    echo_fn(ast2);
    free_AST(ast2);

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

    cr_assert_stdout_eq_str("test \n test");
}