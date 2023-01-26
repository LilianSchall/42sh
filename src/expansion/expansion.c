#include "expansion/expansion.h"

static char *get_var_pid(void)
{
    char *res = mem_malloc(sizeof(char) * 9);
    sprintf(res, "%d", getpid());
    return res;
}

static char *get_all_unquoted(char **argv)
{
    int len = 0;
    int i = 1;
    while (argv[i])
    {
        len += strlen(argv[i]) + 1;
        i++;
    }
    char *result = mem_malloc(len + 1);
    i = 1;
    result[0] = 0;
    while (argv[i])
    {
        strcat(result, argv[i]);
        if (argv[i + 1])
            strcat(result, " ");
        i++;
        // printf("%s\n", result);
    }
    result[len] = 0;
    return result;
}

static char *get_var_aro(char **argv, int quoted)
{
    if (!argv)
        return NULL;
    if (!quoted)
        return get_all_unquoted(argv);

    int len = 0;
    int i = 1;
    while (argv[i])
    {
        len += strlen(argv[i]) + 3;
        i++;
    }

    char *result = mem_malloc(len);
    result[0] = 0;
    i = 1;
    char *quot = mem_calloc(sizeof(char), 2);
    quot[0] = -1;

    while (argv[i])
    {
        strcat(result, quot);
        strcat(result, argv[i]);
        strcat(result, quot);
        if (argv[i + 1])
            strcat(result, " ");
        i++;
        // printf("%s\n", result);
    }
    mem_free(quot);
    result[len - 2] = 0;
    memmove(result, result + 1, len - 1);
    return result;
}

static char *get_var_star(char **argv, int quoted)
{
    if (!argv)
        return NULL;
    if (!quoted)
        return get_all_unquoted(argv);

    int len = 0;
    int i = 1;
    while (argv[i])
    {
        len += strlen(argv[i]) + 1;
        i++;
    }
    char *result = mem_malloc(len);
    i = 1;
    result[0] = 0;
    while (argv[i])
    {
        strcat(result, argv[i]);
        strcat(result, " ");
        i++;
        // printf("%s\n", result);
    }
    result[len - 1] = 0;
    return result;
}

static char *get_var_sharp(char **argv)
{
    int i = 0;
    if (argv)
    {
        while (argv[i])
            i++;
    }
    char *res = mem_malloc(sizeof(char) * 9);
    sprintf(res, "%d", i - 1);
    return res;
}

static char *get_var_random(void)
{
    char *res = mem_malloc(sizeof(char) * 9);
    sprintf(res, "%d", rand() % 32767);
    return res;
}

static char *get_var_uid(void)
{
    char *res = mem_malloc(sizeof(char) * 9);
    sprintf(res, "%d", getuid());
    return res;
}

static char *get_var_n(const char *name, char **argv)
{
    if (!argv)
        return NULL;
    for (int i = 0; name[i]; i++)
    {
        if (name[i] < '0' || name[i] > '9')
            return NULL;
    }
    int i = atoi(name);
    int index = 0;
    while (argv[index])
    {
        if (i == index)
            return argv[i];
        index++;
    }
    return NULL;
}

static char *get_var_tilde(void)
{
    return gc_strdup(getenv("HOME"));
}

static char *get_spec_var(const char *name, char **argv, int quoted)
{
    // printf("is quoted = %d\n", quoted);
    // printf("special var name = %s\n", name);
    if (!strcmp(name, "@"))
        return get_var_aro(argv, quoted);
    else if (!strcmp(name, "*"))
        return get_var_star(argv, quoted);
    else if (!strcmp(name, "$"))
        return get_var_pid();
    else if (!strcmp(name, "#"))
        return get_var_sharp(argv);
    else if (!strcmp(name, "RANDOM"))
        return get_var_random();
    else if (!strcmp(name, "UID"))
        return get_var_uid();
    else
        return get_var_n(name, argv);
    return NULL;
}

static void get_end(const char **str, const char **end, int brackets)
{
    if (brackets)
    {
        while (**end && **end != '}')
            (*end)++;
        if (**end == '}')
            (*end)++;
        else
            *end = *str;
    }
    else
    {
        while (**end && !is_ifs(**end) && !isspace(**end) && **end != '$')
            (*end)++;
        if (*(*end) == '$')
            (*end)++;
    }
}

char *expand_var(const char *str, char **argv, int quoted)
{
    char *result = mem_malloc(strlen(str) + 1);
    char *p = result;
    while (*str)
    {
        if (*str == '$' && *(str + 1) && !isspace(*(str + 1)))
        {
            str++;
            // Find the variable name and end
            int brackets = 0;
            if (*str == '{')
                brackets = 1;

            const char *var_name = str + brackets;
            const char *end = var_name;
            
            get_end(&str, &end, brackets);
            
            char *tmp = strndup(var_name, end - var_name - brackets);
            char *var = getenv(tmp);

            if (!var)
                var = get_spec_var(tmp, argv, quoted);

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
            else // No variable found, remove the ${variable} or $variable
                str = end;
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

char *expand_symbol_array(struct symbol **values, char **argv)
{
    char *result = mem_malloc(strlen(values[0]->value) + 1);
    char *p = result;
    for (int i = 0; values[i] != NULL; i++)
    {
        char *expanded = NULL;
        if (values[i]->is_expandable)
        {
            if (!strcmp("~", values[i]->value))
                expanded = get_var_tilde();
            else
            {
                expanded = expand_var(values[i]->value, argv, values[i]->is_double_quoted);
            }
        }
        else
        {
            expanded = gc_strdup(values[i]->value);
        }
        int len = strlen(expanded);
        if (values[i]->is_double_quoted || values[i]->is_single_quoted)
        {
            len += 2;
            char *tmp = mem_malloc(len + 1);
            tmp[0] = -1;
            tmp[1] = 0;
            strcat(tmp, expanded);
            tmp[len - 1] = -1;
            tmp[len] = 0;
            mem_free(expanded);
            expanded = tmp;
        }

        int cur_len = p - result;
        result = mem_realloc(result, cur_len + len + 1);
        p = result + cur_len;
        memcpy(p, expanded, len);
        p += len;
        mem_free(expanded);
    }
    *p = '\0';
    return result;
}
