#include <criterion/criterion.h>

#include "linked_list/linked_list.h"
#include "lexer/lexer.h"

extern void test_token(struct linked_node **node, const char *sym, enum token_type type);

Test(lexer, echo_foo_pipe_cat_zero_inf_file)
{
    char input[] = "echo foo | cat 0< file";

    struct linked_list *token_list = build_token_list(input);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "echo", WORD);
    test_token(&node, "foo", WORD);
    test_token(&node, "|", R_PIPE);
    test_token(&node, "cat", WORD);
    test_token(&node, "0", IO_NUMBER);
    test_token(&node, "<", R_INF);
    test_token(&node, "file", WORD);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}

Test(lexer, echo_foo_pipe_cat_inf_file)
{
    char input[] = "echo foo | cat < file";

    struct linked_list *token_list = build_token_list(input);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "echo", WORD);
    test_token(&node, "foo", WORD);
    test_token(&node, "|", R_PIPE);
    test_token(&node, "cat", WORD);
    test_token(&node, "<", R_INF);
    test_token(&node, "file", WORD);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}

Test(lexer, echo_foo_pipe_cat_zero_space_inf_file)
{
    char input[] = "echo foo | cat 0 < file";

    struct linked_list *token_list = build_token_list(input);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "echo", WORD);
    test_token(&node, "foo", WORD);
    test_token(&node, "|", R_PIPE);
    test_token(&node, "cat", WORD);
    test_token(&node, "0", WORD);
    test_token(&node, "<", R_INF);
    test_token(&node, "file", WORD);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}
