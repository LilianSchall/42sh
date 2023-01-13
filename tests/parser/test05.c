#include <criterion/criterion.h>
#include <stddef.h>

#include "AST/AST.h"
#include "linked_list/linked_list.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

extern void depth_test_AST(struct AST *tree, enum AST_type types[], size_t size,
        size_t *i);
extern void test_AST(struct AST *tree, enum AST_type types[], size_t size);

Test(parser, for_i_in_1_2_3_do_echo_foo_done)
{
    char input[] = "for i in 1 2 3\n\n\n\n do echo foo; done";

    struct linked_list *token_list = build_token_list(input);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = { SEQUENCE, CONDITION, ARG, ITER, ARG, ARG, ARG, SEQUENCE, COMMAND, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    deep_free_list(token_list, free_token);
}

Test(parser, for_i_semicolon_do_echo_foo_done)
{
    char input[] = "for i ;\n\n do echo foo; done";

    struct linked_list *token_list = build_token_list(input);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = { SEQUENCE, CONDITION, ARG, ITER, ARG, SEQUENCE, COMMAND, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    deep_free_list(token_list, free_token);
}

Test(parser, for_i_in_do_echo_foo_done)
{
    char input[] = "for i in; do echo foo; done";

    struct linked_list *token_list = build_token_list(input);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = { SEQUENCE, CONDITION, ARG, ITER, SEQUENCE, COMMAND, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    deep_free_list(token_list, free_token);
}
