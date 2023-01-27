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

    char *tmp = NULL;
    if (value_ast->type == ARG)
    {
        tmp = expand_symbol_array(value_ast->value->values, env->argv);
        for (int i = 0; tmp[i]; i++)
        {
            if (is_ifs(tmp[i]))
                    tmp[i] = ' ';
        }
    }
    // the child is a subshell -> subshell and take stdout as value
    else if (value_ast->type == D_SUBSHELL) 
    {
        tmp = execute_AST_D_SUBSHELL(value_ast, env);
    }
    else // arithmethic expansion
    {
        tmp = evalexpr(value_ast->value->values[0]->value); 
    }
    setenv(var_name, tmp, 1);
    mem_free(tmp);
    return ret_val;
}
