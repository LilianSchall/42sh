#include "commons/commons.h"

// take a string in parameter and return a argv of all words
static char **split_string(char *str)
{
    // fprintf(stderr, "%s$\n", str);
    char **result = mem_calloc(sizeof(char *), 2);
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
            result[i+1] = NULL;
            while (isspace(*p))
                p++;
            start = p;
        }
        else
        {
            if (is_ifs(*p) && !isspace(*p))
                *p = ' ';
            p++;
        }
    }
    if (*(p - 1) && isspace(*(p - 1)))
        return result;
    int len = p - start;
    result[i] = mem_malloc(len + 1);
    memcpy(result[i], start, len);
    result[i][len] = 0;
    i = 0;
    return result;
}

// transform a linked list into a argc (char **)
// argc is a pointer (it will be updated with the length of argv)
// last elem of argv is NULL
char **new_argv(struct AST *tree, int *argc, struct env *env)
{
    struct linked_list *temp = tree->linked_list;

    if (!temp || list_size(temp) == 0)
    {
        return mem_calloc(sizeof(char*), 1);
    }

    struct linked_node *ln = temp->head;

    int len = 0;
    char *str = mem_calloc(sizeof(char*), 1);
    while (ln)
    {
        struct AST *child = ln->data;
        char *tmp = NULL;
        if (child->type == D_SUBSHELL)
            tmp = execute_AST_D_SUBSHELL(child, env);
        else if (child->type == ARITH)
            tmp = evalexpr(child->value->values[0]->value);
        else
            tmp = expand_symbol_array(child->value->values, env->argv);
        if (!tmp)
            continue;
        len = strlen(str) + strlen(tmp);
        str = mem_realloc(str, len + 2);
        strcat(str, tmp);
        str[len] = ' ';
        str[len + 1] = 0;

        mem_free(tmp);
        ln = ln->next;
    }
    str[len] = 0;
    // printf("%s\n", str);
    char **argv = split_string(str);
    mem_free(str);
    for (*argc = 0; argv[*argc]; *argc += 1);
    return argv;
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
