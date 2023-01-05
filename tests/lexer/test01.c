#include <criterion/criterion.h>

#include "linked_list/linked_list.h"
#include "lexer/lexer.h"

static struct token *get_token(void *data)
{
    struct token *token = data;
    return token;
}

static void test_token(struct linked_node **node, const char *sym, enum token_type type)
{
    cr_expect_str_eq(get_token((*node)->data)->symbol, sym);
    cr_expect_eq(get_token((*node)->data)->type, type);
    *node = (*node)->next;
}

Test(lexer, test_echo_foo)
{
    char input[] = "echo foo";

    struct linked_list *token_list = build_token_list(input);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "echo", WORD);
    test_token(&node, "foo", WORD);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}
#if 0
Test(lexer, test_echo_foo_semicolon)
{
    char input[] = "echo foo;";

    struct linked_list *token_list = build_token_list(input);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "echo", WORD);
    test_token(&node, "foo", WORD);
    test_token(&node, ";", SEMICOLON);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}

#endif
