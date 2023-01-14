#include "parsing_tools.h"

struct token *consume_token(struct linked_list *token_list,
                            enum token_type type, struct log_info info)
{
    struct token *token = list_head(token_list);

    if (!token)
    {
        if (info.trigger_warn)
            warnx("Missing word %s at %s", info.sym, info.rulename);
        return NULL;
    }

    if (token->type != type)
    {
        if (info.trigger_warn)
            warnx("%s: %s missmatch at %s", token->symbol, info.sym,
                  info.rulename);
        return NULL;
    }

    list_pop(token_list);

    return token;
}

struct AST *combine_token_AST(struct AST *tree, struct token *token)
{
    if (tree == NULL)
        return NULL;

    tree->value = token;
    return tree;
}

void purge_newline_token(struct linked_list *token_list)
{
    // swipe up any \n token clean

    struct token *token = list_head(token_list);

    while (token != NULL && token->type == NEWLINE)
    {
        list_pop(token_list);
        free_token(token);
        token = list_head(token_list);
    }
}

void blend_sequence_AST(struct AST *tree, struct AST *child)
{
    if (tree->type != SEQUENCE)
        return;

    if (child->type != SEQUENCE)
    {
        list_append(tree->linked_list, child);
        return;
    }

    struct AST *new = list_head(child->linked_list);

    while (new)
    {
        list_pop(child->linked_list);
        blend_sequence_AST(tree, new);
        new = list_head(child->linked_list);
    }
    free_AST(child);
}

struct AST *root_sequence_if_needed(struct AST *tree)
{
    if (!tree || tree->type == SEQUENCE)
        return tree;

    struct AST *seq = new_AST(NULL, SEQUENCE, new_list());

    list_append(seq->linked_list, tree);

    return seq;
}
