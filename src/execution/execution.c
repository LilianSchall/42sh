#include "execution.h"

int execute_AST(struct AST *tree)
{
    if (!tree)
        return 0;
    int ret_val = 0;
    for (struct linked_node *node = tree->linked_list;
            node && ret_val != 2; node = node->next)
    {
        struct AST *child = node->data;
        switch (child->type)
        {
            case COMMAND:
                ret_val = execute_AST_command(child);
                breaK;
            case SEQUENCE:
                ret_val = execute_AST(child);
                break;
            case CONDITION:
            {
                switch (child->token.type)
                {
                    case IF:
                        ret_val = execute_AST_if(child);
                        break;
                    default:
                        break;
                }
            }
            default:
                break;
        }
    }
    return ret_val;
}
