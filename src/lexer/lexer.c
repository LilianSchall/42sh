#include "lexer.h"

static struct token *parse_quoted_word(char **word_begin_ptr, 
        bool *reading_quote, char **input);
static struct token *parse_unquoted_word(char **word_begin_ptr,
        bool *reading_quote, char **input);
static void skip_char(char **stream, int offset);
static void offset_char(char **stream, int offset);
static char *get_word(char **word_begin_ptr, char **input);
static struct token *create_token(char **word_begin_ptr, char **input,
        char **token_value);
static void execute_parsing(struct linked_list *token_list, char **word_begin_ptr,
        char **input, bool *reading_quote);

struct token *create_token(char **word_begin_ptr, char **input, 
        char **token_value)
{
    char *symbol = get_word(word_begin_ptr, input);

    int index = find_special_tokens(symbol, token_value);

    //here index serves as an enum
    return new_token(symbol, index == -1 ? WORD : index);
}

// this function skips a char by replacing it by -1 
// and goes by offset char to the right
void skip_char(char **stream, int offset)
{
    **stream = -1;
    offset_char(stream, offset);
}

// this function makes an offset to the right of the given value
void offset_char(char **stream, int offset)
{
    *stream += offset;
}

// this function makes the neccesary operations to duplicate the word
// enclosed by the two pointers
char *get_word(char **word_begin_ptr, char **input)
{
    int offset = *word_begin_ptr + 1 < *input  ? 0 : 1;
    char tmp = GETCHAR(input, offset);
    GETCHAR(input, offset) = '\0';
    char *word = my_strdup(*word_begin_ptr);
    GETCHAR(input, offset) = tmp;

    return word;
}

struct token *parse_quoted_word(char **word_begin_ptr,
        bool *reading_quote,char **input)
{
    if (!(*word_begin_ptr))
    {
        if (GETCHAR(input, 0) == '\'') // word is ''
            *reading_quote = false;
        else 
            *word_begin_ptr = *input;
            // as we are quoted, every character including spaces are part of the word
            // so our start of the word is here

        // we return NULL because we haven't parse any token at all right now
        return NULL;
    }

    // else we are currently reading a word
    // we stop reading a word if we encounter a single quote
    
    if (GETCHAR(input, 0) == '\'') // we found the matching quote
    {
        if (!GETCHAR(input,1)) // if we are at the end of the input
        {
            // we parse the last token
            **input = '\0';
            
            char *symbol = my_strdup(*word_begin_ptr);
            **input = '\'';

            // in quoted mode, every token is a word
            *reading_quote = false;
            return new_token(symbol, WORD);
        }
        
        //else if we are not at the end and the next char is not a space
        if (!isspace(GETCHAR(input, 1)))
        {
            // we haven't finished reading our whole token, 
            // so we skip the the quote and continue reading
            skip_char(input,1);

            // we skip the next quote too if there is any and stay in quote mode
            if (GETCHAR(input, 1) == '\'')
                skip_char(input,0);
            else
                *reading_quote = false;
        }
        else
        {
            *reading_quote = false;
            return create_token(word_begin_ptr, input, NULL);
        }
    } 

    return NULL;
}

struct token *parse_unquoted_word(char **word_begin_ptr, 
        bool *reading_quote, char **input)
{
    static CREATE_DICO(token_value);
    static CREATE_DELIMITATORS(delims);

    if (!(*word_begin_ptr)) // if we are not currently parsing a new word
    {
        if (isspace(GETCHAR(input, 0))) // we omit spaces
            return NULL;

        // else we find the new begin of our word
        *word_begin_ptr = *input;
    }
    
    if (find_delims(GETCHAR(input, 0), delims) == -1)
        return NULL;
    
    if (GETCHAR(input, 0) == 0)
        return create_token(word_begin_ptr, input, token_value); 

    // the current char is a delimitator
    // if it is an escape char
    if (GETCHAR(input, 0) == '\\')
    {
        // we escape it
        skip_char(input, !isspace(GETCHAR(input,1)) ? 1 : 0);
        return NULL;
    }
    
    // else if two same delimitators are following each other 
    if (!isspace(GETCHAR(input, 0)) && 
            GETCHAR(input, 0) == GETCHAR(input, 1) &&
            *input == *word_begin_ptr)
        // then it is not a delimitator but a special token
        // so we wait to reach the end of this special token
    {
        return NULL;
    }

    // if we encounter a single quote while already parsing a word
    // we omit the quote by putting its slot to -1
    // and juste continue reading the word but this time in quoted mode
    // when we will create the token, we will read the whole word again
    // and delete the -1 from the token
    if (GETCHAR(input, 0) == '\'')
    {
        skip_char(input,0);
        *reading_quote = true;
        return NULL;
    }

    // now we have a token
    // that is at the end no matter what

    struct token *token = create_token(word_begin_ptr, input, token_value);
    

    if (*input > *word_begin_ptr + 1)
        offset_char(input, -1);

    return token;
}

void execute_parsing(struct linked_list *token_list, char **word_begin_ptr,
        char **input, bool *reading_quote)
{
    struct token *current_token = NULL;

    // we based our parsing on wheter we read a quoted word or not
    if (*reading_quote)
        current_token = parse_quoted_word(word_begin_ptr, reading_quote,
                input);
    else
        current_token = parse_unquoted_word(word_begin_ptr, reading_quote,
                input);

    // if there is a new token
    if (current_token)
    {
        // we add it to list
        token_list = list_append(token_list, current_token);
        *word_begin_ptr = NULL;
    }
}

struct linked_list *build_token_list(char *input)
{
    if (strlen(input) == 0)
        return NULL;

    bool reading_quote = false;

    char *word_begin_ptr = NULL;

    struct linked_list *token_list = new_list();
    
    for (; *input != '\0'; input++)
        execute_parsing(token_list, &word_begin_ptr, &input, &reading_quote);

    if (reading_quote) // single_quote missmatch
    {
        deep_free_list(token_list, free_token);
        return NULL;
    }

    if (word_begin_ptr)
        execute_parsing(token_list, &word_begin_ptr, &input, &reading_quote);
    


    return token_list;
}
