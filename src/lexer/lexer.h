#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>

#include "commons/commons.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

#define GETCHAR(Stream, Nb) (*(*Stream + Nb))

struct lexer_states
{
    bool *reading_quote;
    bool *reading_comm;
};

// this function builds the token_list based on the given input
// returns NULL if input is NULL (wrong execution)
// omits shell commands and shebangs
// interpret every element between single quotes as words
struct linked_list *build_token_list(char *input);

#endif /* !LEXER_H */
