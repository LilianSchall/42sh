#include "token.h"

struct token *new_token(char *symbol, enum token_type type, bool is_expandable)
{
    struct token *token = malloc(sizeof(struct token));

    if (!token)
        return NULL;

    token->symbol = symbol;
    token->type = type;
    token->is_expandable = is_expandable;

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
    static CREATE_DICO(dico);
    struct linked_node *node = NULL;
    size_t i = 0;

    for (node = list->head; node; node = node->next)
    {
        struct token *token = node->data;

        printf("%lu. token: %s ", i, token->symbol);
        printf("token type: ");
        if (token->type == WORD)
            printf("word");
        else if (token->type == IO_NUMBER)
            printf("io_number");
        else if (token->type == NEWLINE)
            printf("newline");
        else
            printf("%s", dico[token->type]);
        i++;
        printf(" is_expandable: %d\n", token->is_expandable); 
    }
}

bool is_redirect(struct token *token)
{
    CREATE_REDIRECT_SCOUT(types);

    size_t len = sizeof(types) / sizeof(enum token_type);

    for (size_t i = 0; i < len; i++)
    {
        if (types[i] == token->type)
            return true;
    }

    return false;
}
