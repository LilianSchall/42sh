#include "expansion.h"

#if 0

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

char *get_spec_var(char ***res, const char *name, int quoted)
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
#endif

char **expand_var(const char *str, int quoted)
{
    char **result = mem_malloc(sizeof(char*) * 2);
	int index = 0;
	result[0] = mem_malloc(strlen(str) + 1);
    result[1] = NULL;
    char *p = result[0];
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
            char **var = mem_malloc(sizeof(char*) * 2); 
			var[0] = getenv(tmp);
            var[1] = NULL;
            mem_free(tmp);

            //if (!var[0])
            //    get_spec_var(&var, var_name, quoted);

			int var_index = 0;
            while (var[var_index])
            {
                // Replace the variable with its value
                int len = strlen(var[var_index]);
                int cur_len = p - result[index];
                result[index] = mem_realloc(result[index], cur_len + len + 1);
				// +strlen(end) ?
                p = result[index] + cur_len;
                memcpy(p, var[var_index], len);
                p += len;
				*p = '\0';
                str = end;
				index++;
				result = mem_realloc(result, sizeof(char*) * (index + 2));
				var_index++;
            }
            if (!var_index)
            {
                // No variable found, remove the ${variable} or $variable
                str = end;
            }
            mem_free(var);
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
    if (index)
        result[index] = NULL;
    return result;
}

char **expand_symbol_array(const struct symbol **values)
{
    int count = 0;
    for (int i = 0; values[i] != NULL; i++)
	{
        if (values[i]->is_expandable)
		{
            char **expanded = expand_var(values[i]->value, 0);
            for (int j = 0; expanded[j] != NULL; j++)
            {
                mem_free(expanded[j]);
                count++;
            }
            mem_free(expanded);
        }
        else
            count++;
    }
    char **result = mem_malloc(sizeof(char *) * (count + 1));
    int index = 0;
    for (int i = 0; values[i] != NULL; i++)
	{
        if (values[i]->is_expandable)
		{		
            char **expanded = expand_var(values[i]->value, 0);
            for (int j = 0; expanded[j] != NULL; j++)
			{
                result[index] = expanded[j];
                index++;
            }
            mem_free(expanded);
        }
        else
		{
            int len = strlen(values[i]->value);
            result[index] = mem_malloc(len + 1);
            memcpy(result[index], values[i]->value, len);
            result[index][len] = '\0';
            index++;
        }
    }
    result[count] = NULL;
    return result;
}
