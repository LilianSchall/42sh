#include "execution/execution.h"

static int execute_AST_if(struct AST *tree, struct env *env)
{
    struct linked_node *child = tree->linked_list->head;
    struct AST *cond = child->data;
    child = child->next;
    struct AST *t = child->data;
    if (execute_AST_main(cond, env) == 0) // cond is true, we execute the 'then'
    {
        return execute_AST_main(t, env);
    }
    else
    {
        if (child->next) // cond is false, we execute the 'else'
            return execute_AST_main(child->next->data, env);
    }
    return 0;
}

// exec a while or until command
// if val_cond = 0 -> while
// if val_cond = 1 -> until
static int execute_AST_while_until(struct AST *tree, int val_cond,
                                   struct env *env)
{
    int return_val = 0;
    int while_cond = val_cond;

    struct linked_node *child = tree->linked_list->head;
    struct AST *cond = child->data;

    child = child->next;
    struct AST *bloc = child->data;

    while (while_cond == val_cond && check_status())
    {
        while_cond = execute_AST_main(cond, env); // check condition

        if (while_cond == val_cond)
            return_val = execute_AST_main(bloc, env); // exec commands

        if (status && status->continue_val > 0)
            status->continue_val -= 1;
    }

    if (status && status->break_val > 0)
        status->break_val -= 1;

    return return_val;
}

static int execute_AST_for(struct AST *tree, struct env *env)
{
    int ret_val = 0;
    struct linked_node *child = tree->linked_list->head;
    struct AST *ast_arg = child->data;
    char *var_name = ast_arg->value->values[0]->value;

    child = child->next; // should not be NULL (check here if error occurs)
    struct AST *ast_iter = child->data;

    // create iter table, should expand var and subshells
    int argc = 0;
    char **iter_args = new_argv(ast_iter, &argc, env);

    child = child->next; // should not be NULL either
    struct AST *ast_seq = child->data;

    int i = 0;
    while (iter_args[i] && check_status())
    {
        setenv(var_name, iter_args[i], 1);
        ret_val = execute_AST_main(ast_seq, env);
        i++;
        if (status && status->continue_val > 0)
            status->continue_val -= 1;
    }
    if (status && status->break_val > 0)
        status->break_val -= 1;

    free_argv(iter_args);

    return ret_val;
}

int execute_AST_condition(struct AST *tree, struct env *env)
{
    int ret_val = 0;
    switch (tree->value->type)
    {
    case IF:
        ret_val = execute_AST_if(tree, env);
        break;
    case WHILE:
        increase_nb_loop();
        ret_val = execute_AST_while_until(tree, 0, env); // while is true
        decrease_nb_loop();
        break;
    case UNTIL:
        increase_nb_loop();
        ret_val = execute_AST_while_until(tree, 1, env); // until is true
        decrease_nb_loop();
        break;
    case FOR:
        increase_nb_loop();
        ret_val = execute_AST_for(tree, env);
        decrease_nb_loop();
        break;
    default:
        break;
    }
    return ret_val;
}
