#include "commons.h"

static size_t my_strlen(char *str)
{
    // this function will only count the chars between 0 and 128
    size_t len = 0;
    size_t i = 0;
    while (str[i] != '\0')
    {
        if (str[i] != -1)
            len++;
    }

    return len;
}

char *my_strdup(char *str)
{
    size_t len = my_strlen(str);

    char *new_str = calloc(len + 1, 1);

    if (!new_str)
        return NULL;

    size_t j = 0;
    for (size_t i = 0; str[i]; i++)
    {
        if (str[i] != -1)
            new_str[j++] = str[i];
    }

    return new_str;
}

int find_special_tokens(char *str, char **special_tokens)
{
    if (str == NULL)
        return -1;
    
    for (size_t i = 0; special_tokens[i] != NULL; i++)
    {
        if (!strcmp(str, special_tokens[i]))
            return i;
    }

    return -1;
}

int find_delims(char c, char *delims)
{
    for (size_t i = 0; delims[i]; i++)
    {
        if (c == delims[i])
            return i;
    }

    return -1;
}

