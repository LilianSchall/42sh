#include <criterion/criterion.h>

#include "linked_list/linked_list.h"
#include "lexer/lexer.h"

extern void test_token(struct linked_node **node, const char *sym, enum token_type type);

Test(lexer, echo_quote_double_quote)
{
    char input[] = "echo \"'coucou'\"";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "echo", WORD);
    test_token(&node, "'coucou'", WORD);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}

Test(lexer, until_true_do_echo_dollar_foo)
{
    char input[] = "until true; do echo $foo \";\" ; done";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "until", UNTIL);
    test_token(&node, "true", WORD);
    test_token(&node, ";", SEMICOLON);
    test_token(&node, "do", DO);
    test_token(&node, "echo", WORD);
    test_token(&node, "$foo", WORD);
    test_token(&node, ";", WORD);
    test_token(&node, ";", SEMICOLON);
    test_token(&node, "done", DONE);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}
