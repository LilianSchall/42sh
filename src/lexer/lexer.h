#ifndef LEXER_H
#define LEXER_H

#include "linked_list/linked_list.h"
#include "token/token.h"

// this function builds the token_list based on the given input
// returns NULL if input is NULL (wrong execution)
// omits shell commands and shebangs
// interpret every element between single quotes as words
struct linked_list *build_token_list(char *input);

#endif /* !LEXER_H */