#include "evalexpr_ast.h"

void ast_print(const struct node *root)
{
    if (!root)
        return;
    ast_print(root->left_child);
    if (root->type == NUMBER)
        printf("%d", root->value);
    else
        printf("%c", root->type ? '*' : '+');
    ast_print(root->right_child);
}

int power(int a, int b)
{
    int acc = 1;
    for (int i = 0; i < b; i++)
        acc *= a;
    return acc;
}

int ast_evaluate(const struct node *root, int *err)
{
    if (root->type == ADD)
    {
        if (root->unary)
            return ast_evaluate(root->right_child, err);
        return ast_evaluate(root->left_child, err)
            + ast_evaluate(root->right_child, err);
    }
    if (root->type == SUB)
    {
        if (root->unary)
            return -ast_evaluate(root->right_child, err);
        return ast_evaluate(root->left_child, err)
            - ast_evaluate(root->right_child, err);
    }
    if (root->type == MUL)
    {
        return ast_evaluate(root->left_child, err)
            * ast_evaluate(root->right_child, err);
    }
    if (root->type == DIV)
    {
        int r = ast_evaluate(root->right_child, err);
        if (!r)
        {
            *err = 3;
            return 0;
        }
        return ast_evaluate(root->left_child, err) / r;
    }
    if (root->type == MOD)
    {
        int r = ast_evaluate(root->right_child, err);
        if (!r)
        {
            *err = 3;
            return 0;
        }
        return ast_evaluate(root->left_child, err) % r;
    }
    else if (root->type == POW)
    {
        int res = ast_evaluate(root->right_child, err);
        if (res < 0)
        {
            *err = 3;
            return 0;
        }
        return power(ast_evaluate(root->left_child, err), res);
    }
    return root->value;
}
