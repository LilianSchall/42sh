#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

#include <stdio.h>

void print_argv(int argc, char **argv)
{
    printf("size : %d\n-----", argc);

    for(int i = 0; i < argc; i++)
    {
        printf("\n%s", argv[i]);
    }
    printf("\n----\n");

}

int main(void)
{
    struct AST *ast_word = new_AST(new_token(copy_string("test \\n test"), (enum token_type) WORD), (enum AST_type) ARG, NULL);

    struct linked_list *ll_ast = new_list();
    ll_ast = list_append(ll_ast, ast_word);
    
    struct AST *ast = new_AST(new_token(copy_string("-en"), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast);

    struct linked_list *ll_ast2 = new_list();
    ll_ast2 = list_append(ll_ast2, ast);
    
    struct AST *ast2 = new_AST(new_token(copy_string("echo"), (enum token_type) WORD), (enum AST_type) COMMAND, ll_ast2);

    struct linked_list *ll = get_linked_list_from_AST(ast2);

    int argc = 0;

    char **argv = new_argv(ll, &argc);
    
    print_argv(argc, argv);

    printf("-- result --\n");
    echo_fn(argc, argv);

    free_argv(argc, argv);
    free_list(ll);
    free_AST(ast2);

    return 0;
}