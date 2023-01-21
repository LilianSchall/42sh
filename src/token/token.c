#include "token.h"

struct token *new_token(struct symbol **values, enum token_type type)
{
    struct token *token = mem_malloc(sizeof(struct token));

    if (!token)
        return NULL;

    token->values = values;
    token->type = type;

    return token;
}

void free_token(void *data)
{
    if (!data)
        return;

    struct token *token = data;

    free_symbol_array(token->values);
    mem_free(token);
}

void print_token_list(struct linked_list *list)
{
    static CREATE_DICO(dico);
    struct linked_node *node = NULL;
    size_t i = 0;

    for (node = list->head; node; node = node->next)
    {
        struct token *token = node->data;

        printf(" %lu. token:", i);
        print_symbols(token->values);
        printf("token type: ");
        if (token->type == WORD)
            printf("word");
        else if (token->type == IO_NUMBER)
            printf("io_number");
        else if (token->type == NEWLINE)
            printf("newline");
        else if (token->type == VARASSIGNMENT)
            printf("varassignment");
        else
            printf("%s", dico[token->type]);
        i++;
        printf("\n");
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
