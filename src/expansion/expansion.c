#include "expansion.h"

char *expand_var(const char *str)
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

            if (var)
            {
                // Replace the variable with its value
                int len = strlen(var);
                int cur_len = p - result;
                result = realloc(result, cur_len + len + strlen(end) + 1);
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

#if 0
char *expand_var(const char *str)
{
    char *result = malloc(strlen(str) + 1);
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
#endif

