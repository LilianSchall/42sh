#include <criterion/criterion.h>
#include <stddef.h>

#include "AST/AST.h"
#include "linked_list/linked_list.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

extern void depth_test_AST(struct AST *tree, enum AST_type types[], size_t size,
        size_t *i);
extern void test_AST(struct AST *tree, enum AST_type types[], size_t size);

Test(parser, while_echo_foo_semicolon_do_echo_prout_echo_file_done)
{
    char input[] = "while 'echo' \"foo\"\ndo echo prout\necho 'file';done";

    struct linked_list *token_list = build_token_list(input);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = { SEQUENCE, CONDITION, SEQUENCE, COMMAND, ARG, ARG, SEQUENCE, COMMAND, ARG, ARG, COMMAND, ARG, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    deep_free_list(token_list, free_token);
}

Test(parser, while_true_do_echo_foo_done_sup_file)
{
    char input[] = "while true\ndo\necho foo; done> file";

    struct linked_list *token_list = build_token_list(input);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = { SEQUENCE, REDIRECTION, ARG, CONDITION, SEQUENCE, COMMAND, ARG, SEQUENCE, COMMAND, ARG, ARG, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    deep_free_list(token_list, free_token);
}

Test(parser, for_i_semicolon_do_echo_foo_done)
{
    char input[] = "for i ;\n\n do echo foo; done";

    struct linked_list *token_list = build_token_list(input);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = { SEQUENCE, CONDITION, ARG, ITER, ARG, SEQUENCE, COMMAND, ARG, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    deep_free_list(token_list, free_token);
}

Test(parser, for_i_in_do_echo_foo_done)
{
    char input[] = "for i in; do echo foo; done";

    struct linked_list *token_list = build_token_list(input);

    struct AST *tree = build_shell_AST(token_list);

    enum AST_type types[] = { SEQUENCE, CONDITION, ARG, ITER, SEQUENCE, COMMAND, ARG, ARG};

    test_AST(tree, types, sizeof(types) / sizeof(enum AST_type));

    free_AST(tree);
    deep_free_list(token_list, free_token);
}
