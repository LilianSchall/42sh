#include "commons.h"

static size_t my_strlen(char *str, int type)
{
    // this function will only count the chars between 0 and 128
    size_t i = 0;
    size_t len = 0;

    if (type != 0)
        i++;

    while (str[i] > 0 || str[i] == -1)
    {
        if (str[i] > 0) // if it is a char and not a marker
            len++; // then we count it as part of the symbol
        i++;
    }

    return len;
}

static int type_of_subword(char *str)
{
    // if the string is unquoted then return 0
    if (str[0] >= 0)
        return 0;

    // else return the quote marker
    return str[0];
}

int my_strdup(char *str, struct symbol *sym)
{
    int type = type_of_subword(str);

    size_t len = my_strlen(str, type);

    char *symbol = mem_calloc(len + 1, 1);

    size_t j = 0;
    size_t i = 0;
    for (; j < len; i++)
    {
        if (str[i] >= 0)
        {
            symbol[j++] = str[i];
        }
    }

    sym->value = symbol;
    sym->is_expandable =
        type != SINGLE_QUOTE_MARKER && type != DELIMITER_MARKER;
    sym->is_double_quoted = type == DOUBLE_QUOTE_MARKER;
    sym->is_single_quoted = type == SINGLE_QUOTE_MARKER;

    return i + (str[i] == type ? 1 : 0);
}

int find_special_tokens(char *str, char **special_tokens)
{
    if (str == NULL || special_tokens == NULL)
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
    size_t i = 0;
    for (; delims[i]; i++)
    {
        if (c == delims[i])
            return i;
    }

    if (c == '\0')
        return i;

    return -1;
}

// check if the string start a the index pos by the string occ
// exemple ("test ab", 2, "st") -> true
int str_start(char *string, int pos, char *occ)
{
    int i = 0;
    while (string[pos] != '\0' && occ[i] != '\0')
    {
        if (string[pos] != occ[i])
            return 0;
        i++;
        pos++;
    }

    return occ[i] == '\0';
}

char *str_replace(char *string, char *occ, char *c)
{
    int occ_len = strlen(occ);
    int c_len = strlen(c);
    int diff = c_len - occ_len;
    int i = 0;

    int size_of_string = strlen(string);

    while (i < size_of_string && string[i] != '\0')
    {
        if (str_start(string, i, occ))
        {
            if (diff > 0)
            {
                size_of_string += diff;

                string =
                    mem_realloc(string, sizeof(char) * (size_of_string + 1));

                for (int j = size_of_string; j - diff >= i; j--) // move
                    string[j] = string[j - diff];
            }

            else if (diff < 0)
            {
                int diff2 = -1 * diff;

                for (int j = i; j + diff2 <= size_of_string; j++)
                    string[j] = string[j + diff2]; // move

                size_of_string -= diff2;
                // string = realloc(string, sizeof(char) * (size_of_string +
                // 1));
            }
            for (int j = 0; j < c_len; j++)
                string[i + j] = c[j]; // replace

            i += (diff > 0 ? diff : -diff);
        }
        else
            i++;
    }
    return string;
}

char *copy_string(char *src)
{
    int l = strlen(src);
    char *dest = mem_malloc(sizeof(char) * (l + 1));
    return strcpy(dest, src);
}

int my_itoa(char *string)
{
    int val = 0;
    int i = 0;
    while (string[i] != '\0')
    {
        if (string[i] < '0' || string[i] > '9')
            return -1;

        val = val * 10 + string[i] - '0';
        i++;
    }
    return val;
}

int is_ifs(char c)
{
    char *ifs = getenv("IFS");
    if (!ifs)
        return 0;
    for (int i = 0; ifs[i]; i++)
    {
       if (c == ifs[i])
            return 1;
    }
    return 0;
}
