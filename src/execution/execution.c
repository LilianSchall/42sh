#include "execution.h"

int execute_AST_cmd(struct AST *tree)
{
    char *cmd = tree->value->symbol;
    if (!strcmp("echo", cmd))
        return 0; // put echo function
    int pid = fork();
    if (!pid)
    {
        execl("/bin/sh", "42sh", "-c", cmd, NULL);
    }
    int status = 0;
    wait(&status);
    return status;
}

int execute_AST_if(struct AST *tree)
{
    struct linked_node *child = tree->linked_list->head;
    struct AST *cond = child->data;
    child = child->next;
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
    return 0;
}

int execute_AST(struct AST *tree)
{
    if (!tree)
        return 0;
    int ret_val = 0;
    for (struct linked_node *node = tree->linked_list->head;
            node; node = node->next)
    {
        struct AST *child = node->data;
        switch (child->type)
        {
            case COMMAND:
                ret_val = execute_AST_cmd(child);
                break;
            case SEQUENCE:
                ret_val = execute_AST(child);
                break;
            case CONDITION:
            {
                switch (child->value->type)
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
