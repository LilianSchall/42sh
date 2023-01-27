#include "evalexpr.h"

char *evalexpr(char *str)
{
    int err = 0;
    struct node *tree = ast_build(str, &err);
    if (err)
    {
        ast_delete(tree);
        return NULL;
    }
    int res = ast_evaluate(tree, &err);
    if (err)
    {
        ast_delete(tree);
        return NULL;
    }
    ast_delete(tree);
    char *result = mem_malloc(sizeof(char *) * 17);
    sprintf(result, "%d", res);
    return result;
}
