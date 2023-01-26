#include "parser.h"

struct AST *assignment_word_rule(struct linked_list *token_list,
                                 bool trigger_warn)
{
    struct token *token = list_head(token_list);

    if (!token || token->type != VARASSIGNMENT)
    {
        if (trigger_warn)
            warnx("missing word VARASSIGNMENT in assignment_word_rule");
        return NULL;
    }

    list_pop(token_list);

    char *str = get_cat_symbols(token->values);

    if (!strcmp(str, "="))
    {
        mem_free(str);
        free_token(token);
        return NULL;
    }

    char *name = strtok(str, "=");
    char *value = strtok(NULL, "\0");

    struct AST *tree = new_AST(NULL, ASSIGNMENT, new_list());

    list_append(tree->linked_list,
                new_AST(new_token(new_unique_symbols(gc_strdup(name), false,
                                                     false, false),
                                  WORD),
                        ARG, NULL));

    list_append(
        tree->linked_list,
        new_AST(new_token(new_unique_symbols(gc_strdup(value ? value : ""),
                                             true, false, false),
                          WORD),
                ARG, NULL));

    mem_free(str);
    free_token(token);

    return tree;
}
