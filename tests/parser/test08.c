#include <criterion/criterion.h>
#include <stddef.h>

#include "AST/AST.h"
#include "linked_list/linked_list.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

extern void depth_test_AST(struct AST *tree, enum AST_type types[], size_t size,
        size_t *i);
extern void test_AST(struct AST *tree, enum AST_type types[], size_t size);

Test(parser, case_100_full_parenthesing)
{
    char input[] = "case 100 in\n( 10 ) echo in10;;\n ( 20 | 100 ) echo good;;\n ( * ) echo default;;\nesac";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = { SEQUENCE, CASE_C, ITER, ARG, ITER, ARG, SEQUENCE, COMMAND, ARG, ARG, ITER, ARG, ARG, SEQUENCE, COMMAND, ARG, ARG, ITER, SEQUENCE, COMMAND, ARG, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    deep_free_list(token_list, free_token);
}
