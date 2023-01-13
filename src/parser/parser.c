#include "parser.h"

struct AST *build_shell_AST(struct linked_list *token_list)
{
    struct AST *tree = input_rule(token_list);

    if (!tree)
        return NULL;

    if (tree->type == SEQUENCE)
        return tree;

    struct AST *sequence = new_AST(NULL, SEQUENCE, new_list());

    list_append(sequence->linked_list, tree);

    return sequence;
}
