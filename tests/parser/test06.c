#include <criterion/criterion.h>
#include <stddef.h>

#include "AST/AST.h"
#include "linked_list/linked_list.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

extern void depth_test_AST(struct AST *tree, enum AST_type types[], size_t size,
        size_t *i);
extern void test_AST(struct AST *tree, enum AST_type types[], size_t size);

Test(parser, echo_foo_pipe_echo_baz)
{
    char input[] = "echo foo | echo baz";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = { SEQUENCE, PIPE, SEQUENCE, COMMAND, ARG, ARG, SEQUENCE, COMMAND, ARG, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    deep_free_list(token_list, free_token);
}

Test(parser, echo_foo_pipe_echo_baz_pipe_head)
{
    char input[] = "echo foo | echo baz | head";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = { SEQUENCE, PIPE, SEQUENCE, COMMAND, ARG, ARG, SEQUENCE, COMMAND, ARG, ARG, SEQUENCE, COMMAND, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    deep_free_list(token_list, free_token);
}

Test(parser, neg_echo_foo_pipe_echo_baz)
{
    char input[] = "! echo foo | echo baz | head";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = { SEQUENCE, OPERATOR, PIPE, SEQUENCE, COMMAND, ARG, ARG, SEQUENCE, COMMAND, ARG, ARG, SEQUENCE, COMMAND, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    deep_free_list(token_list, free_token);
}
