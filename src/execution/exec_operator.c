#include "execution/execution.h"

int execute_AST_operator(struct AST *tree, char **argv)
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
        ret_val = !execute_AST_main(child, argv);
    }
    else if (type == AND) // && condition
    {
        ret_val = execute_AST_main(child, argv);

        if (ret_val == 1)
            return 1;

        return execute_AST_main(child2, argv);
    }
    else if (type == OR) // || condition
    {
        ret_val = execute_AST_main(child, argv);
        if (ret_val == 0)
            return 0;

        return execute_AST_main(child2, argv);
    }

    return ret_val;
}