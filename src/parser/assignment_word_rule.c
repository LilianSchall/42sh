#include "parser.h"

struct AST *assignment_word_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct token *token = list_head(token_list);

    if (!token || token->type != VARASSIGNMENT)
    {
        if (trigger_warn)
            warnx("missing word VARASSIGNMENT in assignment_word_rule");
        return NULL;
    }

    list_pop(token_list);

    char *name = strdup(strtok(token->symbol, "="));
    char *value = strdup(strtok(NULL, "="));

    struct AST *tree = new_AST(NULL, ASSIGNMENT, new_list());

    list_append(tree->linked_list, new_AST(new_token(name, WORD, false), ARG, NULL));
    list_append(tree->linked_list, new_AST(new_token(value, WORD, false), ARG, NULL));

    free_token(token);

    return tree;
}
