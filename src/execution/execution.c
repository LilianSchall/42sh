#include "execution.h"

int not_builtin_fn(int argc, char **argv)
{   
    if (argc == 0)
        return 0;

    int ret_val = 0;
    int pid = fork();

    if (!pid)
        execvp(argv[0], argv);

    ret_val = 0;
    wait(&ret_val);

    return ret_val;
}

int execute_AST_cmd(struct AST *tree)
{
    char *cmd = tree->value->symbol;
    int ret_val = 0;

    struct linked_list *ll_word = get_linked_list_from_AST(tree);    
    int argc = 0;
    char **argv = new_argv(ll_word, &argc);
    
    if (!strcmp("echo", cmd)) // builtin command
    {
        ret_val = echo_fn(argc, argv);
    }
    else if (!strcmp("true", cmd)) // true
    {
        ret_val = true_fn(argc, argv);
    }
    else if (!strcmp("false", cmd)) // false
    {
        ret_val = false_fn(argc, argv);
    }
    else 
    {
        ret_val = not_builtin_fn(argc, argv); // not a builtin command
    }

    free_argv(argc, argv);
    free_list(ll_word);

    return ret_val;
}

int execute_AST_if(struct AST *tree)
{
    struct linked_node *child = tree->linked_list->head;
    struct AST *cond = child->data;
    child = child->next;
    struct AST *t = child->data;
    if (execute_AST(cond) == 0) // cond is true, we execute the 'then'
    {
        return execute_AST(t);
    }
    else
    {
        if (child->next) // cond is false, we execute the 'else'
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
