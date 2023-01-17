#include "expansion.h"

struct var spec_var = {.argc = 0, .argv = NULL};

void set_spec_var(int argc, char **argv)
{
    spec_var.argc = argc;
    spec_var.argv = argv;
}

char *get_var_pid(void)
{
    char *res = mem_malloc(sizeof(char) * 9);
    sprintf(res, "%d", getpid());
    return res;
}

char *get_all_unquoted()
{

}

char *get_var_aro(int quoted)
{
    if (!quoted)
        return get_all_unquoted();
    for (int i = 0; i < spec_var.argc; i++)
    {

    }
}

char *get_var_star(int quoted)
{
    return NULL;
}

char *get_var_sharp(void)
{
    char *res = mem_malloc(sizeof(char) * 9);
    sprintf(res, "%d", spec_var.argc);
    return res;
}

char *get_var_random(void)
{
    return NULL;
}

char *get_var_uid(void)
{
    return NULL;
}

char *get_var_n(const char *name)
{
    int i = atoi(name);
    if (i < spec_var.argc)
        return spec_var.argv[i];
    return NULL;
}

char *get_spec_var(const char *name, int quoted)
{
    if (strlen(name) == 1)
    {
        switch (name[0])
        {
            case '@':
                return get_var_aro(quoted);
                break;
            case '*':
                return get_var_star(quoted);
                break;
            case '$':
                return get_var_pid();
                break;
            case '#':
                return get_var_sharp();
                break;
            default:
                return get_var_n(name);
                break;
        }
    }
    else if (!strcmp(name, "RANDOM"))
        return get_var_random();
    else if (!strcmp(name, "UID"))
        return get_var_uid();
    else
        return get_var_n(name);
    return NULL;
}

char *expand_var(const char *str, int quoted)
{
    char *result = mem_malloc(strlen(str) + 1);
    char *p = result;
    while (*str)
    {
        if (*str == '$')
        {
            str++;
            // Find the variable name and end
            const char *var_name;
            const char *end;
            int brackets = 0;
            if (*str == '{')
                brackets = 1;

            var_name = str + brackets;
            end = var_name;
            if (brackets)
            {
                while (*end && *end != '}')
                    end++;
                if (*end == '}')
                {
                    end++;
                }
                else
                {
                    end = str;
                }
            }
            else
            {
                while (*end && *end != ' ' && *end != '$')
                    end++;
            }

            char *tmp = strndup(var_name, end - var_name - brackets);
            char *var = getenv(tmp);
            mem_free(tmp);

            if (!var)
                var = get_spec_var(var_name, quoted);

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
                // No variable found, remove the ${variable} or $variable
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
