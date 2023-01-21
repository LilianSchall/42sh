#include "execution.h"

int execute_AST_function(struct AST *tree, struct env *env)
{
    struct linked_node *child = tree->linked_list->head;

    struct AST *ast_fun_name = child->data;
    char *fun_name = ast_fun_name->value->values[0]->value;

    child = child->next;
    struct AST *ast_fun = child->data;

    add_function(env->functions, fun_name, ast_fun);

    return 0;
}
