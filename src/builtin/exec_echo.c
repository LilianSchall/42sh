#include "builtin/builtin.h"

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
int get_echo_option(int *pos, int argc, char **argv, struct echo_option *echo_option)
{
    // get the word
    if(*pos >= argc)
        return 0;
    
    char *word = argv[*pos];
    int index = 0;

    // word is not an option
    if(word[index] == '\0' || word[index] != '-')
        return 0;

    index++;

    char old_e = echo_option->option_e;
    char old_E = echo_option->option_E;
    char old_n = echo_option->option_n;

    while(word[index] != '\0') // check which option is activated
    {
        if (word[index] == 'n')
            echo_option->option_n = 1;
        else if (word[index] == 'E')
        {
            echo_option->option_E = 1;
            echo_option->option_e = 0;

        }
        else if (word[index] == 'e')
        {
            echo_option->option_E = 0;
            echo_option->option_e = 1;

        }
        else
        {
            echo_option->option_n = old_n;
            echo_option->option_E = old_E;
            echo_option->option_e = old_e;

            return 0; // wrong echo option or not supported
        }

        index++;
    }

    // check if next word is also new option
    *pos += 1;
    return get_echo_option(pos, argc, argv, echo_option);
    
}


// will print all words in the list and apply rules on them
int print_echo_words(int *pos, int argc, char **argv, 
        struct echo_option *echo_option)
{
    if (*pos >= argc)
        return 0;
    
    // get the word of the node
    char *word = argv[*pos];

    if(echo_option->option_E || !echo_option->option_e)
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
    *pos += 1;

    if(*pos < argc)
    {
        printf(" ");
        return print_echo_words(pos, argc, argv, echo_option); // print next word
    }

    return 0;
}


// echo  builtin function
// take argc & argv in parameter
int echo_fn(int argc, char **argv)
{
    //get the first child of the 'echo' AST
    // struct AST *new_AST = tree->linked_list->head->data;

    // check which options is/are activated
    struct echo_option *echo_option = new_echo_option();

    int pos = 1;

    int return_value = get_echo_option(&pos, argc, argv, echo_option);

    // print all words
    return_value = print_echo_words(&pos, argc, argv, echo_option);

    if(!echo_option->option_n) // print the final newline
        printf("\n");

    fflush(stdout);

    free(echo_option);

    return return_value;
}
