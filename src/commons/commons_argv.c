#include "commons/commons.h"

// take a string in parameter and return a argv of all words
char **split_string(char *str)
{
    // fprintf(stderr, "%s$\n", str);
    char **result = mem_malloc(sizeof(char *) * 2);
    char *p = str;
    int i = 0;
    while (*p == ' ' || *p == '\n')
        p++;
    char *start = p;
    int quoted = 0;
    while (*p)
    {
        // printf("char: %c(%d), quoted: %d\n", *p, *p, quoted);
        if (*p == -1)
        {
            quoted = !quoted;
            int len = strlen(p + 1);
            if (*(p + 1) == -1)
            {
                quoted = !quoted;
                memmove(p, p + 2, len);
            }
            else
                memmove(p, p + 1, len + 1);
        }
        else if (!quoted && isspace(*p))
        {
            int len = p - start;
            result[i] = mem_malloc(len + 1);
            memmove(result[i], start, len);
            result[i][len] = 0;
            i++;
            result = mem_realloc(result, sizeof(char *) * (i + 2));
            while (*p == ' ' || *p == '\n')
                p++;
            start = p;
        }
        else
            p++;
    }
    if (p == str || isspace(*(p - 1)))
    {
        result[i] = NULL;
        return result;
    }
    int len = p - start;
    result[i] = mem_malloc(len + 1);
    memcpy(result[i], start, len);
    result[i][len] = 0;
    result[i + 1] = NULL;
    i = 0;
    return result;
}

// transform a linked list into a argc (char **)
// argc is a pointer (it will be updated with the length of argv)
// last elem of argv is NULL
char **new_argv(struct AST *tree, int *argc, struct env *env)
{
    struct linked_list *temp = tree->linked_list;

    char **argv_res = mem_malloc(sizeof(char *));

    if (!temp)
    {
        argv_res[0] = NULL;
        return argv_res;
    }

    struct linked_node *ln = temp->head;

    int i = 0;
    while (ln)
    {
        struct AST *child = ln->data;
        char *str = NULL;
        if (child->type == D_SUBSHELL)
            str = execute_AST_D_SUBSHELL(child, env);
        else
            str = expand_symbol_array(child->value->values, env->argv);
        char **tmp = split_string(str);
        mem_free(str);
        int j = 0;
        while (tmp[j])
        {
            argv_res[i] = gc_strdup(tmp[j]);
            i++;
            j++;
            argv_res = mem_realloc(argv_res, sizeof(char *) * (i + 1));
        }
        free_argv(tmp);
        ln = ln->next;
    }
    argv_res[i] = NULL;
    *argc = i;
    return argv_res;
}

// free argc (char **)
void free_argv(char **argv)
{
    for (int i = 0; argv[i]; i++)
    {
        mem_free(argv[i]);
    }
    mem_free(argv);
}
