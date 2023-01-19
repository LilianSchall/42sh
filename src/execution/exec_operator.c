#include "execution/execution.h"

int execute_AST_operator(struct AST *tree)
{
    enum token_type type = tree->value->type;
    int ret_val = 0;

    struct linked_node *node = tree->linked_list->head;
    struct AST *child = node->data;
    struct AST *child2;

    if (node->next)
        child2 = node->next->data;

    if (type == NEG) // ! condition
    {
        ret_val = !execute_AST(child);
    }
    else if (type == AND) // && condition
    {
        ret_val = execute_AST(child);

        if (ret_val == 1)
            return 1;

        return execute_AST(child2);
    }
    else if (type == OR) // || condition
    {
        ret_val = execute_AST(child);
        if (ret_val == 0)
            return 0;

        return execute_AST(child2);
    }

    return ret_val;
}