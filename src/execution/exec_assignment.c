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
    struct AST *value_ast = child->next->data;

    if (value_ast->type == ARG)
    {
        char *tmp = expand_symbol_array(value_ast->value->values, env->argv);
        setenv(var_name, tmp, 1);
        mem_free(tmp);
    }
    else // the child is a sequence -> subshell and take stdout as value
    {
        setenv(var_name, execute_AST_D_SUBSHELL(value_ast, env), 1); 
    }
    return ret_val;
}
