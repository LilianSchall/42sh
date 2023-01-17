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

    char *symbol = mem_calloc(len, 1);

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
    sym->is_expandable = type != SINGLE_QUOTE_MARKER;
    sym->is_double_quoted = type == DOUBLE_QUOTE_MARKER;
    sym->is_single_quoted = type == SINGLE_QUOTE_MARKER;

    return i;
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

            i += (diff > 0 ? diff : -diff) + 1;
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

// transform a linked list into a argc (char **)
// argc is a pointer (it will be updated with the length of argv)
// last elem of argv is NULL
char **new_argv(struct AST *tree, int *argc)
{
    *argc = (int)list_size(tree->linked_list) + 1;
    // int argc_tmp = *argc;

    struct linked_list *temp = tree->linked_list;

    char **argv = mem_malloc(sizeof(char *) * (*argc + 1));

    int i = 1;

    if (tree->value->values[0]->is_expandable)
    {
        argv[0] = expand_var(tree->value->values[0]->value, 0);
        if (!*(argv[0]))
        {
            i--;
            (*argc)--;
        }
    }
    else
        argv[0] = copy_string(tree->value->values[0]->value);

    if (!temp)
    {
        argv[1] = NULL;
        return argv;
    }

    struct linked_node *ln = temp->head;

    for (; i < *argc; i++)
    {
        struct AST *child = ln->data;
        if (child->value->values[0]->is_expandable)
        {
            char *tmp = expand_var(child->value->values[0]->value, 0);
            if (*tmp)
                argv[i] = tmp;
            else
            {
                i--;
                (*argc)--;
            }
        }
        else
            argv[i] = copy_string(child->value->values[0]->value);
        ln = ln->next;
    }

    argv[i] = NULL;
    return argv;
}

// free argc (char **)
void free_argv(int argc, char **argv)
{
    for (int i = 0; i < argc; i++)
    {
        mem_free(argv[i]);
    }
    mem_free(argv);
}

// return a linked list of all 'word' in our AST
// it only takes the left child
// example of usage : get echo words
struct linked_list *get_linked_list_from_AST(struct AST *AST)
{
    struct linked_list *ll_ast = new_list();

    // add command name
    ll_ast = list_append(ll_ast, AST->value->values[0]->value);

    if (!AST->linked_list)
        return ll_ast;

    struct linked_node *node = AST->linked_list->head;

    struct AST *ast_temp;

    while (node) // add all childs
    {
        ast_temp = node->data;
        ll_ast = list_append(ll_ast, ast_temp->value->values[0]->value);
        node = node->next;
    }

    return ll_ast;
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

