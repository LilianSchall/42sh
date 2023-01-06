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
        i++;
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
    while(string[pos] != '\0' && occ[i] != '\0')
    {
        if(string[pos] != occ[i])
            return 0;
        i++;
        pos++;
    }

    return occ[i] == '\0';
}

char *str_replace(char* string, char* occ, char* c) 
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
            if(diff > 0)
            {
                size_of_string += diff;

                string = realloc(string, sizeof(char) * (size_of_string + 1));

                for(int j = size_of_string; j - diff >= i; j--) // move
                    string[j] = string[j-diff];

            }

            else if(diff < 0)
            {
                int diff2 = -1 * diff;

                for(int j = i ; j + diff2 <= size_of_string; j++)
                    string[j] = string[j+diff2]; // move
                
                size_of_string -= diff2;
                //string = realloc(string, sizeof(char) * (size_of_string + 1));
            }
            for(int j = 0; j < c_len; j++)
                string[i+j] = c[j]; // replace

            i += (diff > 0 ? diff : - diff) + 1;
        }
        else
            i++;
    }
    return string;
}

char *copy_string(char *src)
{
    int l = strlen(src);
    char *dest = malloc(sizeof(char) * (l + 1));
    return strcpy(dest, src);
}

// transform a linked list into a argc (char **)
// argc is a pointer (it will be updated with the length of argv)
char **new_argv(struct linked_list *linked_list, int *argc)
{
    *argc = (int) list_size(linked_list);

    struct linked_list *temp = linked_list;

    struct linked_node *ln = temp->head;

    char **argv = malloc(sizeof(char *) * (*argc));

    for(int i = 0; i < *argc; i++)
    {
        argv[i] = copy_string(ln->data);
        ln = ln->next;
    }

    return argv;
}

// free argc (char **)
void free_argv(int argc, char **argv)
{
    for(int i = 0; i < argc; i++)
    {
        if(argv[i])
            free(argv[i]);
    }
    free(argv);
}

// return a linked list of all 'word' in our AST 
// it only takes the left child
// example of usage : get echo words
struct linked_list *get_linked_list_from_AST(struct AST *AST)
{
    struct linked_list *ll_ast = new_list();

    // add command name
    ll_ast = list_append(ll_ast, AST->value->symbol);

    struct linked_node *node = AST->linked_list->head;

    struct AST *ast_temp;

    while(node) // add all childs
    {
        ast_temp = node->data;
        ll_ast = list_append(ll_ast, ast_temp->value->symbol);
        node = node->next;
    }
    
    return ll_ast;
}