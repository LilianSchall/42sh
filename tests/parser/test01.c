#include <criterion/criterion.h>
#include <stddef.h>

#include "AST/AST.h"
#include "linked_list/linked_list.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

void depth_test_AST(struct AST *tree, enum AST_type types[], size_t size,
        size_t *i)
{
    if (*i >= size)
        return;

    // testing the tree
    cr_expect_eq(tree->type, types[*i]);

    *i = *i + 1;
    if (tree->linked_list == NULL)
        return;

    // traversing its children from leftmost to rightmost
    for (struct linked_node *node = tree->linked_list->head; node; 
            node = node->next)
    {
        struct AST *child = node->data;
        depth_test_AST(child, types, size, i);
    }
}

void test_AST(struct AST *tree, enum AST_type types[], size_t size)
{
    size_t i = 0;

    depth_test_AST(tree, types, size, &i);
}

Test(parser, parse_echo_foo)
{
    char input[] = "echo foo";

    struct linked_list *token_list = build_token_list(input);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = { SEQUENCE, COMMAND, ARG, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    deep_free_list(token_list, free_token);
}

Test(parser, parse_echo_foo_foo)
{
    char input[] = "echo foo foo";

    struct linked_list *token_list = build_token_list(input);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = { SEQUENCE, COMMAND, ARG, ARG, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    deep_free_list(token_list, free_token);
}
