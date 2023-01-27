#include "lexer.h"

static struct symbol **add_sym_to_array(struct symbol **symbols,
                                        struct symbol *sym, int *capacity)
{
    int len = 0;

    while (symbols[len])
        len++;

    if (len + 1 >= *capacity)
    {
        *capacity += DEFAULT_NB_SYMBOLS;
        symbols = mem_realloc(symbols, sizeof(struct symbol *) * (*capacity));

        for (int i = len; i < *capacity; i++)
            symbols[i] = NULL;
    }

    symbols[len++] = sym;

    return symbols;
}

// this function makes an offset to the right of the given value
static void offset_char(char **stream, int offset)
{
    *stream += offset;
}

// this function skips a char by replacing it by -1
// and goes by offset char to the right
static void skip_char(char **stream, int offset, int replace_value)
{
    **stream = replace_value;
    offset_char(stream, offset);
}

// this function makes the neccesary operations to create a new symbol
// based on the string enclosed by the two pointers
// it returns the nb of char read
static int get_symbol(char **word_begin_ptr, char **input, struct symbol *sym)
{
    // this offset is used to correctly parse delimitors and operators
    // that are less then 3 char
    int offset = *word_begin_ptr + 1 < *input ? 0 : 1;

    // the char that we will temporarily replace
    char tmp = GETCHAR(input, offset);
    GETCHAR(input, offset) = '\0';
    int char_read = my_strdup(*word_begin_ptr, sym);
    // we replace the temp char
    GETCHAR(input, offset) = tmp;

    return char_read;
}

static bool my_isspace(char c)
{
    return c == ' ' || c == '\t' || c == '\v' || c == '\r' || c == '\f';
}

static struct token *create_token(char **word_begin_ptr, char **input,
                                  char **token_value)
{
    struct symbol sym = { 0 };
    int capacity = DEFAULT_NB_SYMBOLS;
    struct symbol **symbols =
        mem_calloc(DEFAULT_NB_SYMBOLS, sizeof(struct symbol *));

    char *begin = *word_begin_ptr;
    do
    {
        int offset = get_symbol(&begin, input, &sym);
        if (!sym.value)
            break;
        struct symbol *new_sym = mem_malloc(sizeof(struct symbol));
        memcpy(new_sym, &sym, sizeof(struct symbol));

        symbols = add_sym_to_array(symbols, new_sym, &capacity);
        begin += offset;
    } while (begin < *input);

    if (symbols[0] == NULL)
    {
        mem_free(symbols);
        return NULL;
    }

    int index = find_special_tokens(symbols[0]->value, token_value);
    enum token_type type = ERROR;

    if (strstr(symbols[0]->value, "="))
        type = VARASSIGNMENT;
    else if (symbols[1] == NULL) // it means there is only one symbol
    {
        if (index != -1 && symbols[0]->is_expandable)
            type = index;
        else
            type = WORD;
    }
    else
        type = WORD;

    return new_token(symbols, type);
}

static struct token *parse_quoted_word(char **word_begin_ptr, char **input,
                                       struct lexer_states states)
{
    static CREATE_DELIMITATORS(delims);

    if (!(*word_begin_ptr))
    {
        if (GETCHAR(input, 0) == '\'') // word is ''
            *states.reading_quote = false;
        else
            *word_begin_ptr = *input;
        // as we are quoted, every character including spaces are part of the
        // word so our start of the word is here

        // we return NULL because we haven't parse any token at all right now
        return NULL;
    }

    // else we are currently reading a word
    // we stop reading a word if we encounter a single quote

    if (GETCHAR(input, 0) == '\'') // we found the matching quote
    {
        // else if we are not at the end and the next char is not a space
        //  and it isn't a delimitator
        if (!my_isspace(GETCHAR(input, 1))
            && (find_delims(GETCHAR(input, 1), delims) == -1
                || GETCHAR(input, 1) == '\''))
        {
            // we haven't finished reading our whole token,
            // so we skip the the quote and continue reading
            skip_char(input, 1, SINGLE_QUOTE_MARKER);

            // we skip the next quote too if there is any and stay in quote mode
            if (GETCHAR(input, 1) == '\'')
                skip_char(input, 0, SINGLE_QUOTE_MARKER);
            else
                *states.reading_quote = false;
        }
        else
        {
            *states.reading_quote = false;
            skip_char(input, 0, SINGLE_QUOTE_MARKER);
            return create_token(word_begin_ptr, input, NULL);
        }
    }

    return NULL;
}

static bool is_chevron(char c)
{
    return c == '>' || c == '<';
}

static struct token *parse_double_quoted_word(char **word_begin_ptr, char **input,
                                              struct lexer_states states)
{
    static CREATE_DELIMITATORS(delims);
    if (!(*word_begin_ptr))
    {
        if (GETCHAR(input, 0) == '\"') // word is ""
            *states.reading_double_quote = false;
        else
            *word_begin_ptr = *input;
        // as we are double_quoted, every character including spaces are part of
        // the word so our start of the word is here

        // we return NULL because we haven't parse any token at all right now
        return NULL;
    }
    // the current char is a delimitator
    // if it is an escape char
    if (GETCHAR(input, 0) == '\\')
    {
        char next = GETCHAR(input, 1);
        if (next != '$' && next != '`' && next != '"' && next != '\n')
            return NULL;
        // we escape it
        skip_char(input, !isspace(GETCHAR(input, 1)) ? 1 : 0, DELIMITER_MARKER);
        return NULL;
    }

    // else we are currently reading a word
    // we stop reading a word if we encounter a single quote
    if (GETCHAR(input, 0) == '"') // we found the matching quote
    {
        // if we are not at the end and the next char is not a space
        // and it isn't a delimitator
        if (!my_isspace(GETCHAR(input, 1))
            && (find_delims(GETCHAR(input, 1), delims) == -1))
        {
            // we haven't finished reading our whole token,
            // so we skip the the quote and continue reading
            skip_char(input, 0, DOUBLE_QUOTE_MARKER);

            // we skip the next quote too if there is any and stay in quote mode
            if (GETCHAR(input, 1) == '"')
            {
                offset_char(input, 1);
                skip_char(input, 0, DOUBLE_QUOTE_MARKER);
            }
            else
                *states.reading_double_quote = false;
        }
        else
        {
            *states.reading_double_quote = false;
            skip_char(input, 0, DOUBLE_QUOTE_MARKER);
            return create_token(word_begin_ptr, input, NULL);
        }
    }
    return NULL;
}

static struct token *lex_new_token(char **word_begin_ptr, char **input,
        char *delims, char **token_value)
{
    static CREATE_REDIRECTIONS(redirections);

    struct token *token = NULL;
    char tmp[4];
    tmp[0] = GETCHAR(input, 0);
    tmp[1] = GETCHAR(input, 1);
    tmp[2] = GETCHAR(input, 1) != 0 ? GETCHAR(input, 2) : 0;
    tmp[3] = 0;

    // else if two same delimitators are following each other
    if (!isspace(GETCHAR(input, 0))
        && *input == *word_begin_ptr
        && ((GETCHAR(input, 0) == GETCHAR(input, 1)
            || sub_special_tokens(tmp, redirections) != -1)))
    // then it is not a delimitator but a special token
    // so we wait to reach the end of this special token
    {
        return NULL;
    }

    // now we have a token
    // that is at the end no matter what

    if (find_delims(GETCHAR(word_begin_ptr, 0), delims) == -1
        && *input <= *word_begin_ptr + 1)
    {
        // if we do not lex a special $ token
        if (GETCHAR(word_begin_ptr, 0) != '$')
            *input -= 1; // we have to go back by -1

        token = create_token(word_begin_ptr, input, token_value);
        return token;
    }
    else
        token = create_token(word_begin_ptr, input, token_value);

    if (*input > *word_begin_ptr + 1)
        offset_char(input, -1);

    return token;

}

static struct token *parse_unquoted_word(char **word_begin_ptr, char **input,
                                         struct lexer_states states)
{
    static CREATE_DELIMITATORS(delims);
    static CREATE_DICO(token_value);

    if (GETCHAR(input, 0) == '#' && *word_begin_ptr == NULL)
    {
        *states.reading_comm = true;
        return NULL;
    }

    if (!(*word_begin_ptr)) // if we are not currently parsing a new word
    {
        if (my_isspace(GETCHAR(input, 0))) // we omit spaces
            return NULL;

        // else we find the new begin of our word
        *word_begin_ptr = *input;
    }

    // if we found a io_number then we parse it as it is
    if (isdigit(GETCHAR(input, 0)) && is_chevron(GETCHAR(input, 1)))
    {
        // we found a io_number
        struct token *token = NULL;
        char *offset_input = *input + 1;
        if (*word_begin_ptr + 1 < *input)
            token = create_token(word_begin_ptr, &offset_input, token_value);
        else
            token = create_token(word_begin_ptr, input, token_value);
        token->type = IO_NUMBER;
        return token;
    }

    if (find_delims(GETCHAR(input, 0), delims) == -1)
        return NULL;

    // this rule is quite bizarre but I will explain it
    // it is used to make the $ stick to the variable name
    if (GETCHAR(input, 0) == '$' && *input == *word_begin_ptr)
        return NULL;

    if (GETCHAR(input, 0) == 0)
        return create_token(word_begin_ptr, input, token_value);

    // the current char is a delimitator
    // if it is an escape char
    if (GETCHAR(input, 0) == '\\')
    {
        // we escape it
        skip_char(input, !isspace(GETCHAR(input, 1)) ? 1 : 0, DELIMITER_MARKER);
        return NULL;
    }

    // if the token is like foo#foo
    // then it is a whole token and not a comment after the #
    if (GETCHAR(input, 0) == '#' && !isspace(GETCHAR(input, 1)))
        return NULL;

    // if we encounter a single quote while already parsing a word
    // we omit the quote by putting its slot to SINGLE_QUOTE_MARKER
    // and juste continue reading the word but this time in quoted mode
    // when we will create the token, we will read the whole word again
    // and delete the -1 from the token
    if (GETCHAR(input, 0) == '\'')
    {
        skip_char(input, 0, SINGLE_QUOTE_MARKER);
        *states.reading_quote = true;
        return NULL;
    }
    // same thing for double quote, we skip the double quote and enter
    // double_quoted mode
    // the omitted quote will be replaced by a DOUBLE_QUOTE_MARKER
    // at the end the whole word will be regenerated
    else if (GETCHAR(input, 0) == '"')
    {
        skip_char(input, 0, DOUBLE_QUOTE_MARKER);
        *states.reading_double_quote = true;
        return NULL;
    }
    
    return lex_new_token(word_begin_ptr, input, delims, token_value);
}

static struct token *parse_comment(char **input, struct lexer_states states)
{
    if (GETCHAR(input, 0) == '\n')
    {
        *states.reading_comm = false;
        return new_token(
            new_unique_symbols(gc_strdup("\n"), false, false, false), NEWLINE);
    }

    return NULL;
}

static void reset_heredoc_separator(struct lexer_states states)
{
    mem_free(*states.heredoc_separator);
    *states.heredoc_separator = NULL;
    *states.reading_heredoc_separator = false;
}

static struct token *parse_heredocs(char **word_begin_ptr, char **input,
        struct lexer_states states)
{
    struct token *token = NULL;
    static char *tmp = NULL;
    if (!(*word_begin_ptr))
    {
        if (isspace(GETCHAR(input, 0)) && GETCHAR(input, 0) != '\n')
            return NULL;
        *word_begin_ptr = *input + 1; // so we can escape the \n
        tmp = mem_calloc(strlen(*states.heredoc_separator) + 1, 1);
    }
    
    if (GETCHAR(input, 1) == '\0')
    {
        warnx("warning: here-document delimited by end-of-file (wanted '%s')",
                *states.heredoc_separator + 1);
        char *end = *input + 1;
        token = create_token(word_begin_ptr, &end, NULL);
        mem_free(tmp);
        reset_heredoc_separator(states);
        return token;
    }

    strncpy(tmp, *input, strlen(*states.heredoc_separator));

    if (!strcmp(tmp, *states.heredoc_separator))
    {
        // we found the other HEREDOC
        char *end = *input + 1;
        token = create_token(word_begin_ptr, &end, NULL);
        *input += strlen(*states.heredoc_separator) - 1;
        mem_free(tmp);
        reset_heredoc_separator(states);
    }
    return token;
}

static struct token *parse_arith_expr(char **word_begin_ptr, char **input,
        struct lexer_states states)
{
    // we will lex this one in one time
    size_t nb_parentheses = 2; //we already lex the first two parentheses
    *states.reading_arith_expr = false;

    *word_begin_ptr = *input;

    while (nb_parentheses != 0)
    {
        if (GETCHAR(input, 0) == '\0' 
            || (GETCHAR(input, 1) == '\0' && nb_parentheses > 1))
            return NULL;
        char c = GETCHAR(input, 0);
        if (c == '(')
            nb_parentheses++;
        else if (c == ')')
            nb_parentheses--;
        offset_char(input, 1);
    }

    if (GETCHAR(input, -1) != GETCHAR(input, -2))
        return NULL;

    offset_char(input,-2);

    struct token *token = create_token(word_begin_ptr, input, NULL);
    offset_char(input,-1);

    return token;
}

static void create_heredoc_separator(struct token *token, 
                                     struct lexer_states states)
{
    char *tmp = get_cat_symbols(token->values);            
    char *heredoc = mem_calloc(strlen(tmp) + 2, 1);
    strcat(heredoc, "\n");
    strcat(heredoc, tmp);
    mem_free(tmp);
    *states.heredoc_separator = heredoc;
}

static bool execute_lexing(struct linked_list *token_list,
                           char **word_begin_ptr, char **input,
                           struct lexer_states states)
{
    struct token *current_token = NULL;
    if (*states.reading_comm)
        current_token = parse_comment(input, states);
    // we based our parsing on wheter we read a quoted word or not
    else if (*states.reading_quote)
        current_token = parse_quoted_word(word_begin_ptr, input, states);
    else if (*states.reading_double_quote)
        current_token = parse_double_quoted_word(word_begin_ptr, input, states);
    else if (*states.heredoc_separator)
        current_token = parse_heredocs(word_begin_ptr, input, states);
    else if (*states.reading_arith_expr)
    {
        current_token = parse_arith_expr(word_begin_ptr, input, states);
        if (!current_token)
            return true;
    }
    else
        current_token = parse_unquoted_word(word_begin_ptr, input, states);

    // if there is a new token
    if (current_token)
    {
        if (current_token->type == BACKQUOTE)
        {
            if (*states.reading_backquote)
                current_token->type = ENDBACKQUOTE;
            *states.reading_backquote = !*states.reading_backquote;
        }
        else if (*states.reading_heredoc_separator)
        {
            if (!is_non_delimitator(current_token->type))
                return true;
            create_heredoc_separator(current_token, states);
            current_token->type = HEREDOC;
        }
        else if (current_token->type == INF_INF 
                || current_token->type == INF_INF_MIN)
        {
            *states.reading_heredoc_separator = true;
        }
        else if (current_token->type == DOLL_OPEN_PARENTHESE
                && GETCHAR(input, 1) == '(')
        {
            offset_char(input, 1);
            current_token->type = DOLL_OPEN_PARENTHESE_PARENTHESE;
            current_token->values[1] = new_symbol(gc_strdup("("),
                    false, false, false);
            *states.reading_arith_expr = true;
        }
        // we add it to list
        token_list = list_append(token_list, current_token);
        *word_begin_ptr = NULL;
    }

    return false;
}

static void clean_token_list(struct linked_list *token_list)
{
    if (token_list == NULL || token_list->head == NULL)
        return;

    struct token *token = list_head(token_list);
    while (token->type == NEWLINE)
    {
        list_pop(token_list);
        free_token(token);
        token = list_head(token_list);
    }
}

struct linked_list *build_token_list(char *input, int *err)
{
    if (strlen(input) == 0)
        return NULL;

    bool reading_comm = false;
    bool reading_quote = false;
    bool reading_double_quote = false;
    bool reading_backquote = false;
    bool reading_heredoc_separator = false;
    char *heredoc_separator = NULL;
    bool reading_arith_expr = false;

    struct lexer_states states = {
        .reading_quote = &reading_quote,
        .reading_double_quote = &reading_double_quote,
        .reading_comm = &reading_comm,
        .reading_backquote = &reading_backquote,
        .reading_heredoc_separator = &reading_heredoc_separator,
        .heredoc_separator = &heredoc_separator,
        .reading_arith_expr = &reading_arith_expr,
    };

    char *word_begin_ptr = NULL;

    struct linked_list *token_list = new_list();

    for (; *input != '\0'; input++)
    {
        if (execute_lexing(token_list, &word_begin_ptr, &input, states))
        {
            deep_free_list(token_list, free_token);
            if (err)
                *err = 1;
            return NULL;
        }
    }
    if (reading_quote || reading_double_quote
        || reading_backquote) // quote missmatch
    {
        deep_free_list(token_list, free_token);
        if (err)
            *err = 1;
        return NULL;
    }

    if (word_begin_ptr)
        execute_lexing(token_list, &word_begin_ptr, &input, states);

    clean_token_list(token_list);

    return token_list;
}
