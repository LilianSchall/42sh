#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>

#include "commons/commons.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

#define GETCHAR(Stream, Nb) (*(*Stream + Nb))
#define DEFAULT_NB_SYMBOLS 4

struct lexer_states
{
    bool *reading_quote;
    bool *reading_double_quote;
    bool *reading_comm;
    bool *reading_backquote;
    bool *reading_heredoc_separator;
    char **heredoc_separator;
};

// this function builds the token_list based on the given input
// returns NULL if input is NULL (wrong execution)
// omits shell commands and shebangs
// interpret every element between single quotes as words
struct linked_list *build_token_list(char *input, int *err);

#endif /* !LEXER_H */
