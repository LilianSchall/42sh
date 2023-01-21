#include "execution/execution.h"

int execute_AST_assignment(struct AST *tree, struct env *env)
{
    /* do things */
    if (env == NULL)
        env = NULL;

    int ret_val = 1;
    struct linked_node *child = tree->linked_list->head;
    struct AST *var_name_ast = child->data;
    // variable name is the token value of the ast
    char *var_name = var_name_ast->value->values[0]->value;

    // taking second child(cant be NULL)
    struct AST *var_value_ast = child->next->data;

    if (var_value_ast->type == ARG)
    {
        ret_val = setenv(var_name, var_value_ast->value->values[0]->value, 1);
    }
    else // the child is a sequence -> subshell and take stdout as value
    {
        // TODO in step 3 or 4
    }
    return ret_val;
}
