#include <criterion/criterion.h>
#include <stddef.h>

#include "AST/AST.h"
#include "linked_list/linked_list.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

extern void depth_test_AST(struct AST *tree, enum AST_type types[], size_t size,
        size_t *i);
extern void test_AST(struct AST *tree, enum AST_type types[], size_t size);

Test(parser, parse_if_true_then_baba_else_boo)
{
    char input[] = "if true; then echo baba; else echo boo; fi";

    struct linked_list *token_list = build_token_list(input);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = { SEQUENCE, CONDITION, SEQUENCE, COMMAND, SEQUENCE, COMMAND, ARG, SEQUENCE, COMMAND, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    free_list(token_list);
}

Test(parser, parse_if_false_true_echo_a_echo_b_echo_c)
{
    char input[] = "if false; true; then\n echo a\n echo b; echo c; fi";

    struct linked_list *token_list = build_token_list(input);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = {SEQUENCE, CONDITION, SEQUENCE, COMMAND, COMMAND, SEQUENCE, COMMAND, ARG, COMMAND, ARG, COMMAND, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    free_list(token_list);
}

Test(parser, parse_if_false_newline_true_echo_a_echo_b_echo_c)
{
    char input[] = "if false\n true\n then\n echo a\n echo b; echo c fi";

    struct linked_list *token_list = build_token_list(input);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = {SEQUENCE, CONDITION, SEQUENCE, COMMAND, COMMAND, SEQUENCE, COMMAND, ARG, COMMAND, ARG, COMMAND, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    free_list(token_list);
}
