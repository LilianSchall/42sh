#include "functions.h"

struct function
{
    char *name;
    struct AST *ast;
};

struct function *new_function(char *name, struct AST *ast)
{
    struct function *fct = mem_malloc(sizeof(struct function));
    fct->name = gc_strdup(name);
    fct->ast = ast;
    return fct;
}

void free_function(void *data)
{
    struct function *fct = data;
    mem_free(fct->name);
    mem_free(fct);
}

void add_function(struct linked_list *functions, char *name, struct AST *ast)
{
    struct linked_node *fun = functions->head;
    for (; fun; fun = fun->next)
    {
        struct function *function = fun->data;
        if (!strcmp(function->name, name))
        {
            function->ast = ast;
        }
    }
    functions = list_append(functions, new_function(name, ast));
}

int call_function(struct linked_list *functions, char **argv, int *ret_val)
{
    struct linked_node *fun = functions->head;
    for (; fun; fun = fun->next)
    {
        struct function *function = fun->data;
        if (!strcmp(function->name, argv[0]))
        {
            *ret_val = execute_AST(function->ast, argv, functions);
            return 1;
        }
    }
    return 0;
}

int remove_function(struct linked_list *functions, char *name)
{
    struct linked_node *fun = functions->head;
    if (!fun)
        return 0;
    struct function *function = fun->data;
    if (!strcmp(function->name, name))
    {
        free_function(function);
        if (fun->next)
            functions->head = fun->next;
        else
        {
            mem_free(fun);
            functions->head = NULL;
        }
        return 1;
    }
    for (; fun->next; fun = fun->next)
    {
        function = fun->next->data;
        if (!strcmp(function->name, name))
        {
            struct linked_node *tmp = fun->next;
            free_function(function);
            if (fun->next)
                fun->next = fun->next->next;
            else
                fun->next = NULL;
            mem_free(tmp);
            return 1;
        }
    }
    return 0;
}
