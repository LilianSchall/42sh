#include "token.h"

struct token *new_token(char *symbol, enum token_type type)
{
    struct token *token = malloc(sizeof(struct token));

    if (!token)
        return NULL;

    token->symbol = symbol;
    token->type = type;

    return token;
}

void free_token(void *data)
{
    if (!data)
        return;

    struct token *token = data;

    free(token->symbol);
    free(token);
}

void print_token_list(struct linked_list *list)
{
    struct linked_node *node = NULL;
    size_t i = 0;

    for (node = list->head; node; node = node->next)
    {
        struct token *token = node->data;

        printf("%lu. token: %s\n", i, token->symbol);
        i++;
    }
}
