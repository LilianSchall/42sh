#include "commons/commons.h"

// take a string in parameter and return a argv of all words
char **split_string(char *str)
{
    //fprintf(stderr, "%s\n\n", str);
    char **result = mem_malloc(sizeof(char*) * 2);;
    char *p = str;
    int i = 0;
    while (*p == ' ' || *p == '\n')
        p++;
    char *start = p;
    int quoted = 0;
    while (*p)
    {
        //printf("char: %c, quoted: %d\n", *p, quoted);
        if (*p == '"')
        {
            quoted = !quoted;
            int len = strlen(p+1);
            memmove(p, p+1, len + 1);
        }
        else if (!quoted && (*p == ' ' || *p == '\n'))
        {
            int len = p - start;
            result[i] = mem_malloc(len + 1);
            memmove(result[i], start, len);
            result[i][len] = 0;
            i++;
            result = mem_realloc(result, sizeof(char*) * (i + 2));
			while (*p == ' ' || *p == '\n')
                p++;
			start = p;
        }
		else
			p++;
    }
    if (*(p - 1) == ' ' || *(p - 1) == '\n')
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
char **new_argv(struct AST *tree, int *argc, char **current_argv)
{
    struct linked_list *temp = tree->linked_list;

    char **argv = mem_malloc(sizeof(char *));

    if (!temp)
    {
        argv[0] = NULL;
        return argv;
    }

    struct linked_node *ln = temp->head;

    int i = 0;
    while (ln)
    {
        struct AST *child = ln->data;
        char *str = NULL;
        if (child->type == D_SUBSHELL)
            str = execute_AST_D_SUBSHELL(child, current_argv);
        else
            str = expand_symbol_array(child->value->values, current_argv);
        char **tmp = split_string(str);
        mem_free(str);
        int j = 0;
        while (tmp[j])
        {
            argv[i] = gc_strdup(tmp[j]);
            i++;
            j++;
            argv = mem_realloc(argv, sizeof(char*) * (i + 1));
        }
        free_argv(tmp);
        ln = ln->next;
    }
    argv[i] = NULL;
    *argc = i;
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
