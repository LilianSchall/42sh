#include <criterion/criterion.h>

#include "linked_list/linked_list.h"
#include "lexer/lexer.h"

extern void test_token(struct linked_node **node, const char *sym, enum token_type type);

Test(lexer, test_echo_sup_file)
{
    char input[] = "echo foo > file";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "echo", WORD);
    test_token(&node, "foo", WORD);
    test_token(&node, ">", R_SUP);
    test_token(&node, "file", WORD);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}

Test(lexer, test_echo_sup_sup_file)
{
    char input[] = "echo foo >> file";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "echo", WORD);
    test_token(&node, "foo", WORD);
    test_token(&node, ">>", R_SUP_SUP);
    test_token(&node, "file", WORD);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}

Test(lexer, test_echo_two_sup_file)
{
    char input[] = "echo foo 2> file";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "echo", WORD);
    test_token(&node, "foo", WORD);
    test_token(&node, "2", IO_NUMBER);
    test_token(&node, ">", R_SUP);
    test_token(&node, "file", WORD);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}

Test(lexer, test_echo_two_1024_file)
{
    char input[] = "echo foo 1024> file";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "echo", WORD);
    test_token(&node, "foo", WORD);
    test_token(&node, "1024", IO_NUMBER);
    test_token(&node, ">", R_SUP);
    test_token(&node, "file", WORD);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}

Test(lexer, test_echo_two_sup_file_one_inf_test)
{
    char input[] = "echo foo 2> file 1< test";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "echo", WORD);
    test_token(&node, "foo", WORD);
    test_token(&node, "2", IO_NUMBER);
    test_token(&node, ">", R_SUP);
    test_token(&node, "file", WORD);
    test_token(&node, "1", IO_NUMBER);
    test_token(&node, "<", R_INF);
    test_token(&node, "test", WORD);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}
