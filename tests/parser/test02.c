#include <criterion/criterion.h>
#include <stddef.h>

#include "AST/AST.h"
#include "linked_list/linked_list.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

extern void depth_test_AST(struct AST *tree, enum AST_type types[], size_t size,
        size_t *i);
extern void test_AST(struct AST *tree, enum AST_type types[], size_t size);

Test(parser, parse_echo_foo_semicolon)
{
    char input[] = "echo foo;";

    struct linked_list *token_list = build_token_list(input);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = { SEQUENCE, COMMAND, ARG };

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    free_list(token_list);
}

Test(parser, parse_echo_foo_echo_foo)
{
    char input[] = "echo foo; echo foo;";

    struct linked_list *token_list = build_token_list(input);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = { SEQUENCE, COMMAND, ARG, COMMAND, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    free_list(token_list);
}
