#include "execution/exec_echo.h"


// create our echo_option struct and init all options
struct echo_option *new_echo_option(void)
{
    struct echo_option *echo_option = malloc(sizeof(struct echo_option));

    // setup the different 
    echo_option->option_n = 1;
    echo_option->option_E = 1;
    echo_option->option_e = 1;

    return echo_option;
}

// check which option is/are activated in your echo tree
int get_echo_option(struct AST *new_AST, struct echo_option *echo_option)
{
    // get the word
    char *word = new_AST->value->symbol;
    int index = 0;

    // word is not an option
    if(word[index] == '\0' || word[index] != '-')
        return 0;

    index ++;

    while(word[index] != '\0') // check which option is activated
    {
        if (word[index] == 'n')
            echo_option->option_n = 0;
        else if (word[index] == 'E')
            echo_option->option_E = 0;
        else if (word[index] == 'e')
        echo_option->option_e = 0;
        else
        {   
            // may be add a cache here
            return 0; // wrong echo option or not supported
        }
        index++;
    }

    // check if next word is also new option
    new_AST = new_AST->linked_list->head->data;
    return get_echo_option(new_AST, echo_option);
    
}


int print_echo_words(struct AST *new_AST, struct echo_option *echo_option)
{
    if (new_AST == NULL)
        return 0;
    
    // get the word of the node
    char *word = new_AST->value->symbol;

    if (echo_option->option_e) //enable \n, ...
    { 
        printf("%s%c", word, ' ');
    }
    else //disable \n, ...
    {
        for(int i = 0; word[i] != '\0'; i++)
            putchar(word[i]);
    }

    // check if it is not a leaf
    if(new_AST->linked_list && new_AST->linked_list->head &&
        new_AST->linked_list->head->data)
    {
        new_AST = new_AST->linked_list->head->data;
        return get_echo_option(new_AST, echo_option); // print next word
    }

    return 0;
}


int exec_echo(struct AST *tree)
{
    //get the first child of the 'echo' AST
    struct AST *new_AST = tree->linked_list->head->data;

    // check which options is/are activated
    struct echo_option *echo_option = new_echo_option();


    int return_value = print_echo_words(new_AST, echo_option);

    if(!echo_option->option_n) // print the final newline
        printf("\n");

    fflush(stdout);

    free(echo_option);
    return return_value;
}
