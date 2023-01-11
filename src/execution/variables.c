#include "variables.h"

static struct linked_list *variables = NULL;

struct var *new_var(char *name, char *value)
{
    struct var *v = malloc(sizeof(struct var));
    if (!v)
        return NULL;
    v->name = strdup(name);
    v->value = strdup(value);
    return v;
}

void free_var(void *v)
{
    struct var *var = v;
    free(var->name);
    free(var->value);
    free(var);
}

void free_variables(void)
{
    deep_free_list(variables, free_var);
}

void print_variables(void)
{
    if (!variables)
        return;
    for (struct linked_node *v = variables->head; v; v = v->next)
    {
        struct var *variable = v->data;
        printf("%s: %s\n", variable->name, variable->value);
    }
}

int assign_var(char *name, char *val)
{
    if (!variables)
        variables = new_list();
    for (struct linked_node *v = variables->head; v; v = v->next)
    {
        struct var *variable = v->data;
        if (!strcmp(variable->name, name))
        {
            free(variable->value);
            variable->value = strdup(val);
            return 1;
        }
    }
    struct var *n_var = new_var(name, val);
    variables = list_append(variables, n_var);
    return 0;
}

char *get_var(char *name)
{
    if (!variables)
        return NULL;
    for (struct linked_node *v = variables->head; v; v = v->next)
    {
        struct var *variable = v->data;
        if (!strcmp(variable->name, name))
        {
            return variable->value;
        }
    }
    return NULL;
}

void replace_var(char **str)
{
    if (!variables)
        return;

    // Iterate through each character in the string
    char *p = *str;
    while (*p)
    {
        if (*p == '$')
        {
            // Find the variable name
            char *var_name = p + 1;
            char *end = var_name;
            while (*end && *end != ' ' && *end != '$')
                end++;

            // Find the variable value
            struct linked_node *cur = variables->head;
            struct var *v = cur->data;
            while (cur && (strncmp(v->name, var_name, end - var_name) != 0))
            {
                cur = cur->next;
                if (cur)
                    v = cur->data;
            }
            if (cur)
            {
                // Replace the variable with its value
                int len = strlen(v->value);
                memmove(p + len, p + (end - p), strlen(p) - (end - p) + 1);
                memcpy(p, v->value, len);
                p += len;
            }
            else
            {
                // No variable found, remove the $variable
                memmove(p, p + (end - p), strlen(p) - (end - p) + 1);
            }
        }
        else
        {
            // No variable, move to the next character
            p++;
        }
    }
}
