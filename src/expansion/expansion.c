#include "expansion.h"

#if 0
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

void init_variables(void)
{
    variables = new_list();
    assign_var("IFS", " \t\n");
}

void print_variables(void)
{
    for (struct linked_node *v = variables->head; v; v = v->next)
    {
        struct var *variable = v->data;
        printf("%s: %s\n", variable->name, variable->value);
    }
}


int assign_var(char *name, char *val)
{
    setenv(name, val, 1);
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
    return getenv(name);
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
#endif

char* expand_var(const char *str)
{
    char *result = mem_malloc(strlen(str) + 1);
    char *p = result;
    while (*str)
    {
        if (*str == '$')
        {
            // Find the variable name
            const char *var_name = str + 1;
            const char *end = var_name;
            while (*end && *end != ' ' && *end != '$')
                end++;
            char *tmp = strndup(var_name, end - var_name);
            char *var = getenv(tmp);
            mem_free(tmp);

            if (var)
            {
                // Replace the variable with its value
                int len = strlen(var);
                int cur_len = p - result;
                result = mem_realloc(result, cur_len + len + strlen(end) + 1);
                p = result + cur_len;
                memcpy(p, var, len);
                p += len;
                str = end;
            }
            else
            {
                // No variable found, remove the $variable
                str = end;
            }
        }
        else
        {
            // No variable, move to the next character
            *p = *str;
            p++;
            str++;
        }
    }
    *p = '\0';
    return result;
}
