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
    sym->is_expandable = type != SINGLE_QUOTE_MARKER;
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

// take a string in parameter and return a argv of all words
char **split_string(char *str) 
{
    int count = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++) 
    {
        if (isspace(str[i]) && !isspace(str[i + 1])) 
        {
            count++;
        }
    }
    char **result = mem_malloc(sizeof(char *) * (count + 2));
    char *token;
    int i = 0;
    char *str_cpy = strdup(str);
    char *delimiter = " \n\0";
    token = strtok(str_cpy, delimiter);
    while (token != NULL) 
    {
        result[i] = mem_malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(result[i], token);
        token = strtok(NULL, delimiter);
        i++;
    }
    result[i] = NULL;
    mem_free(str_cpy);
    return result;
}


// check if tree is as D_SUBSHELL type
// if it is : it execute the AST and redirect the stdout into a string
// then the string goes into a argv containing all words
int check_dollar_subshell(struct AST *tree, int *i, int *argc, char **argv)
{
    if(tree->type != D_SUBSHELL)
        return 0;
    
    char *string = execute_AST_D_SUBSHELL(tree);
    
    char **temp_argv = split_string(string);

    // to do : merge with actual argv

    mem_free(temp_argv);
    mem_free(string);
    return 1;
}

// transform a linked list into a argc (char **)
// argc is a pointer (it will be updated with the length of argv)
// last elem of argv is NULL
char **new_argv(struct AST *tree, int *argc)
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
            str = execute_AST_D_SUBSHELL(child);
        else
            str = expand_symbol_array(child->value->values);
        char **tmp = split_string(str);
        mem_free(str);
        int j = 0;
        if (tmp[j])
        {
            argv[i] = strdup(tmp[j]);
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

char **str_to_argv(char *str) 
{
    int count = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++) 
    {
        if (isspace(str[i]) && !isspace(str[i + 1])) 
        {
            count++;
        }
    }
    char **result = mem_malloc(sizeof(char *) * (count + 2));
    char *token;
    int i = 0;
    char *str_cpy = copy_string(str);
    char *delimiter = " \n\0";
    token = strtok(str_cpy, delimiter);
    while (token != NULL) 
    {
        result[i] = mem_malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(result[i], token);
        token = strtok(NULL, delimiter);
        i++;
    }
    result[i] = NULL;
    mem_free(str_cpy);
    return result;
}


char *get_content_of_pipe(int pipefd[2])
{
    close(pipefd[1]);
    char buffer[1024];
    char *output = malloc(sizeof(char));
    size_t size = 0;
    size_t i = 0;

    while (read(pipefd[0], buffer, sizeof(char) * 1024) > 0) {
        size += strlen(buffer);
        output = realloc(output, sizeof(char) * (size + 1));
        
        size_t j = i;
        while(i < j + strlen(buffer))
        {
            output[i] = buffer[i - j];
            i++;
        }
    }
    output[i] = '\0';
    close(pipefd[0]);
    return output;
}
