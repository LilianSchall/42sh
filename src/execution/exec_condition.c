#include "execution/execution.h"

static int execute_AST_if(struct AST *tree)
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

// exec a while or until command
// if val_cond = 0 -> while
// if val_cond = 1 -> until
static int execute_AST_while_until(struct AST *tree, int val_cond)
{
    int return_val = 0;
    int while_cond = val_cond;

    struct linked_node *child = tree->linked_list->head;
    struct AST *cond = child->data;

    child = child->next;
    struct AST *bloc = child->data;

    while (while_cond == val_cond && break_val == 0 && continue_val == 0)
    {
        while_cond = execute_AST(cond); // check condition

        if (while_cond == val_cond)
            return_val = execute_AST(bloc); // exec commands
        
        if (continue_val > 0)
            continue_val--;
    }

    if (break_val > 0)
        break_val--;

    return return_val;
}

static int execute_AST_for(struct AST *tree)
{
    int ret_val = 0;
    struct linked_node *child = tree->linked_list->head;
    struct AST *ast_arg = child->data;
    char *var_name = ast_arg->value->values[0]->value;
    
    child = child->next; // should not be NULL (check here if error occurs)
    struct AST *ast_iter = child->data;

    // create iter table, should expand var and subshells
    int argc = 0;
    char **iter_args = new_argv(ast_iter, &argc); 

    child = child->next; // should not be NULL either
    struct AST *ast_seq = child->data;

    int i = 0;
    while (iter_args[i] && break_val == 0 && continue_val == 0)
    {
        setenv(var_name, iter_args[i], 1);
        ret_val = execute_AST(ast_seq);
        i++;
        if (continue_val > 0)
            continue_val--;
    }
    if (break_val > 0)
        break_val--;

    free_argv(iter_args);

    return ret_val;
}

int execute_AST_condition(struct AST *tree)
{
    int ret_val = 0;
    switch (tree->value->type)
    {
    case IF:
        ret_val = execute_AST_if(tree);
        break;
    case WHILE:
        nb_loop++;
        ret_val = execute_AST_while_until(tree, 0); // while is true
        nb_loop--;
        break;
    case UNTIL:
        nb_loop++;
        ret_val = execute_AST_while_until(tree, 1); // until is true
        nb_loop--;
        break;
    case FOR:
        nb_loop++;
        ret_val = execute_AST_for(tree);
        nb_loop--;
        break;
    default:
        break;
    }
    return ret_val;
}