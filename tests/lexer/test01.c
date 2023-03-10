#include <criterion/criterion.h>

#include "linked_list/linked_list.h"
#include "lexer/lexer.h"

static struct token *get_token(void *data)
{
    struct token *token = data;
    return token;
}

static char *get_full_sym(struct symbol **values)
{
    size_t len = 0;

    for (size_t i = 0; values[i]; i++)
    {
        len += strlen(values[i]->value);
    }

    char *str = mem_calloc(len + 1, 1);

    for (size_t i = 0; values[i]; i++)
    {
        strcat(str, values[i]->value);
    }

    return str;
}

void test_token(struct linked_node **node, const char *sym, enum token_type type)
{
    char *str = get_full_sym(get_token((*node)->data)->values);
    cr_expect_str_eq(str, sym);
    cr_expect_eq(get_token((*node)->data)->type, type);

    mem_free(str);

    *node = (*node)->next;
}

Test(lexer, test_echo_foo)
{
    char input[] = "echo foo";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "echo", WORD);
    test_token(&node, "foo", WORD);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}

Test(lexer, test_echo_foo_semicolon)
{
    char input[] = "echo foo;";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "echo", WORD);
    test_token(&node, "foo", WORD);
    test_token(&node, ";", SEMICOLON);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}

Test(lexer, if_echo_true_then_echo_foo_fi)
{
    char input[] = "if echo a\n then echo foo; fi";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "if", IF);
    test_token(&node, "echo", WORD);
    test_token(&node, "a", WORD);
    test_token(&node, "\n", NEWLINE);
    test_token(&node, "then", THEN);
    test_token(&node, "echo", WORD);
    test_token(&node, "foo", WORD);
    test_token(&node, ";", SEMICOLON);
    test_token(&node, "fi", FI);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}

Test(lexer, if_cond_and_cond_then_echo_else_echo_fi)
{
    char input[] = "if echo true&& false;then echo foo; else echo fada ; fi";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "if", IF);
    test_token(&node, "echo", WORD);
    test_token(&node, "true", WORD);
    test_token(&node, "&&", AND);
    test_token(&node, "false", WORD);
    test_token(&node, ";", SEMICOLON);
    test_token(&node, "then", THEN);
    test_token(&node, "echo", WORD);
    test_token(&node, "foo", WORD);
    test_token(&node, ";", SEMICOLON);
    test_token(&node, "else", ELSE);
    test_token(&node, "echo", WORD);
    test_token(&node, "fada", WORD);
    test_token(&node, ";", SEMICOLON);
    test_token(&node, "fi", FI);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}

Test(lexer, test_echo_foo_semicolon_quoted)
{
    char input[] = "echo 'foo ;'";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "echo", WORD);
    test_token(&node, "foo ;", WORD);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}

Test(lexer, test_echo_foo_semicolon_quoted_followed)
{
    char input[] = "'echo' 'foo ;'baz";

    struct linked_list *token_list = build_token_list(input, NULL);

    struct linked_node *node = token_list->head;
    
    test_token(&node, "echo", WORD);
    test_token(&node, "foo ;baz", WORD);
    cr_expect_eq(node, NULL);

    deep_free_list(token_list, free_token);
}
