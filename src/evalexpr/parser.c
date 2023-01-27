#include <ctype.h>
#include <stddef.h>
#include <stdio.h>

#include "evalexpr_ast.h"
#include "fifo.h"
#include "lifo.h"

struct node *create_node(int value, enum node_type type)
{
    struct node *node = mem_malloc(sizeof(struct node));
    node->type = type;
    node->value = value;
    node->unary = 0;
    node->left_child = NULL;
    node->right_child = NULL;
    return node;
}

struct fifo *tokenize(const char *str, int *err)
{
    struct fifo *fifo = fifo_init();
    for (; *str; str++)
    {
        if (*str >= '0' && *str <= '9')
        {
            int n = 0;
            while (*str >= '0' && *str <= '9')
            {
                n *= 10;
                n += *str - '0';
                str++;
            }
            fifo_push(fifo, create_node(n, NUMBER));
        }
        if (!(*str))
            break;
        if (*str == '+')
            fifo_push(fifo, create_node(0, ADD));
        else if (*str == '-')
            fifo_push(fifo, create_node(0, SUB));
        else if (*str == '*')
        {
            if (*(str + 1) == '*')
            {
                str++;
                fifo_push(fifo, create_node(0, POW));
            }
            fifo_push(fifo, create_node(0, MUL));
        }
        else if (*str == '/')
            fifo_push(fifo, create_node(0, DIV));
        else if (*str == '%')
            fifo_push(fifo, create_node(0, MOD));
        else if (*str == '(')
            fifo_push(fifo, create_node(0, LB));
        else if (*str == ')')
            fifo_push(fifo, create_node(0, RB));
        else
        {
            if (!isspace(*str))
            {
                fifo_destroy(fifo);
                *err = 1;
                return NULL;
            }
        }
    }
    return fifo;
}

void set_err(int *err, struct fifo *fifo1, struct fifo *fifo2)
{
    *err = 2;
    fifo_destroy(fifo1);
    fifo_destroy(fifo2);
}

struct fifo *check_expr(struct fifo *fifo, int *err)
{
    struct node *lastnode = NULL;
    struct fifo *out = fifo_init();
    int brackets = 0;
    while (fifo_head(fifo))
    {
        struct node *node = fifo_pop(fifo);
        if (node->type == LB)
            brackets += 1;
        if (node->type == RB)
            brackets -= 1;
        if (node->type == NUMBER)
        {
            if ((lastnode && lastnode->type < ADD && lastnode->type != LB)
                || (fifo_head(fifo) && fifo_head(fifo)->type < RB))
            {
                set_err(err, fifo, out);
                mem_free(node);
                return NULL;
            }
        }
        if (node->type >= ADD
            && (!fifo_head(fifo) || fifo_head(fifo)->type == RB))
        {
            set_err(err, fifo, out);
            mem_free(node);
            return NULL;
        }
        if (node->type >= ADD
            && (!lastnode
                || (lastnode->type >= ADD || lastnode->type == LB)))
        {
            if (!fifo_head(fifo) || node->type >= MUL)
            {
                set_err(err, fifo, out);
                mem_free(node);
                return NULL;
            }
            node->unary = 1;
        }
    }
    if (brackets)
    {
        set_err(err, fifo, out);
        return NULL;
    }
    fifo_destroy(fifo);
    return out;
}

struct lifo *get_output_stack_infix(char *str, int *err)
{
    struct fifo *fifo = tokenize(str, err);
    if (*err)
        return NULL;
    fifo = check_expr(fifo, err);
    if (*err)
        return NULL;
    struct lifo *s = lifo_init();
    struct lifo *out = lifo_init();
    while (fifo_head(fifo))
    {
        struct node *node = fifo_pop(fifo);
        if (node->type == NUMBER)
            lifo_push(out, node);
        if (node->unary)
        {
            lifo_push(s, node);
            continue;
        }
        if (node->type == ADD || node->type == SUB)
        {
            while (lifo_head(s)
                   && (lifo_head(s)->type >= ADD || lifo_head(s)->unary))
                lifo_push(out, lifo_pop(s));
            lifo_push(s, node);
        }
        if (node->type >= MUL && node->type <= MOD)
        {
            while (lifo_head(s)
                   && (lifo_head(s)->type >= MUL || lifo_head(s)->unary))
                lifo_push(out, lifo_pop(s));
            lifo_push(s, node);
        }
        if (node->type == POW || node->type == LB)
        {
            while (lifo_head(s) && lifo_head(s)->unary)
                lifo_push(out, lifo_pop(s));
            lifo_push(s, node);
        }
        if (node->type == RB)
        {
            while (lifo_head(s) && lifo_head(s)->type != LB)
                lifo_push(out, lifo_pop(s));
            mem_free(lifo_pop(s));
            mem_free(node);
        }
    }
    while (lifo_head(s))
    {
        lifo_push(out, lifo_pop(s));
    }
    mem_free(fifo);
    mem_free(s);
    return out;
}

struct node *build_tree(struct lifo *s, int *err)
{
    if (!lifo_head(s))
    {
        *err = 2;
        return NULL;
    }
    struct node *node = lifo_pop(s);
    if (node->type == NUMBER)
        return node;
    if (node->unary)
    {
        node->right_child = build_tree(s, err);
        return node;
    }
    node->right_child = build_tree(s, err);
    node->left_child = build_tree(s, err);
    return node;
}

struct node *ast_build(char *str, int *err)
{
    struct lifo *out;
    out = get_output_stack_infix(str, err);
    if (*err)
        return NULL;
    struct node *tree = build_tree(out, err);
    lifo_destroy(out);
    if (*err)
        return NULL;
    return tree;
}

void ast_delete(struct node *root)
{
    if (!root)
        return;
    ast_delete(root->right_child);
    ast_delete(root->left_child);
    mem_free(root);
}
