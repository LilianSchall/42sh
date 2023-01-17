#include <criterion/criterion.h>

#include "linked_list/linked_list.h"
#include "lexer/lexer.h"

extern void test_token(struct linked_node **node, const char *sym, enum token_type type);

Test(lexer, while_true_semicolon_do_echo_prout_done)
{
    char input[] = "while true; do echo prout done";

    struct linked_list *token_list = build_token_list(input);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "while", WHILE);
    test_token(&node, "true", WORD);
    test_token(&node, ";", SEMICOLON);
    test_token(&node, "do", DO);
    test_token(&node, "echo", WORD);
    test_token(&node, "prout", WORD);
    test_token(&node, "done", DONE);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}

Test(lexer, test_equal_thomas_semicolon_echo_dollar_tes)
{
    char input[] = "test=thomas;echo $test";

    struct linked_list *token_list = build_token_list(input);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "test=thomas", VARASSIGNMENT);
    test_token(&node, ";", SEMICOLON);
    test_token(&node, "echo", WORD);
    test_token(&node, "$test", WORD);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}
