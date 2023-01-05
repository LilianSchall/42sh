#include "execution.h"

int execute_AST_cmd(struct AST *tree)
{
    char *cmd = tree->token.symbol;
    if (!strcmp("echo", cmd))
        return echo(tree);
    int pid = fork();
    if (!pid)
    {
        execl("/bin/sh", "42sh", "-c", cmd, NULL);
    }
}

int execute_AST_if(struct AST *tree)
{
    struct linked_node *child = tree->linked_list->head;
    struct AST *cond = child->data;
    child = child->next;
    if (child)
    {
        struct AST *t = child->data;
        if (execute_AST(cond))
        {
            return execute_AST(t);
        }
        else
        {
            if (child->next)
                return execute_AST(child->next->data);
        }
    }
    else
        return execute_AST(cond);
    return 0;
}

int execute_AST(struct AST *tree)
{
    if (!tree)
        return 0;
    int ret_val = 0;
    for (struct linked_node *node = tree->linked_list->head;
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
