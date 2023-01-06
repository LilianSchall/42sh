#include "builtin/exec_echo.h"


struct linked_list *get_argv_signe_AST(struct AST *new_AST)
{
    struct linked_list *ll_ast = new_list();

    while(new_AST)
    {
        ll_ast = list_append(ll_ast, new_AST->value->symbol);
        new_AST = list_head(new_AST->linked_list);
    }
    
    return ll_ast;
}


// create our echo_option struct and init all options
struct echo_option *new_echo_option(void)
{
    struct echo_option *echo_option = malloc(sizeof(struct echo_option));

    // setup the different 
    echo_option->option_n = 0;
    echo_option->option_E = 0;
    echo_option->option_e = 0;

    return echo_option;
}

// check which option is/are activated in your echo tree
int get_echo_option(struct linked_list *ast_word_list, struct echo_option *echo_option)
{
    // get the word
    char *word = list_head(ast_word_list);
    int index = 0;

    // word is not an option
    if(word[index] == '\0' || word[index] != '-')
        return 0;

    index++;

    while(word[index] != '\0') // check which option is activated
    {
        if (word[index] == 'n')
            echo_option->option_n = 1;
        else if (word[index] == 'E')
            echo_option->option_E = 1;
        else if (word[index] == 'e')
            echo_option->option_e = 1;
        else
        {   
            // may be add a cache here
            return 0; // wrong echo option or not supported
        }
        index++;
    }

    // check if next word is also new option
    ast_word_list = list_pop(ast_word_list);
    return get_echo_option(ast_word_list, echo_option);
    
}


// will print all words in the list and apply rules on them
int print_echo_words(struct linked_list *ast_word_list, 
        struct echo_option *echo_option)
{
    if (ast_word_list == NULL || ast_word_list->head == NULL)
        return 0;
    
    // get the word of the node
    char *word = list_head(ast_word_list);

    if(!echo_option->option_e && echo_option->option_E)
    {
        printf(word);
    }
    else  //apply -e
    {   
        word = str_replace(word,"\\n","\n");
        word = str_replace(word,"\\t","\t");
        word = str_replace(word,"\\\\","\\");
        printf(word);
    }

    // check if it is not a leaf
    if(ast_word_list->head->next)
    {
        ast_word_list = list_pop(ast_word_list);
        printf(" ");
        return print_echo_words(ast_word_list, echo_option); // print next word
    }

    return 0;
}


// echo  builtin function
// take an AST as parameter
int echo_fn(struct AST *tree)
{
    //get the first child of the 'echo' AST
    struct AST *new_AST = tree->linked_list->head->data;

    // get a linked list of all words of the AST
    struct linked_list *ast_word_list = get_argv_signe_AST(new_AST);

    // check which options is/are activated
    struct echo_option *echo_option = new_echo_option();
    int return_value = get_echo_option(ast_word_list, echo_option);

    // print all words
    return_value = print_echo_words(ast_word_list, echo_option);

    if(!echo_option->option_n) // print the final newline
        printf("\n");

    fflush(stdout);

    free(echo_option);
    free_list(ast_word_list);

    return return_value;
}
