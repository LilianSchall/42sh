#ifndef PARSER_H
#define PARSER_H

#include <err.h>
#include <stdbool.h>
#include <string.h>

#include "AST/AST.h"
#include "linked_list/linked_list.h"
#include "parsing_tools.h"
#include "rules.h"
#include "token/token.h"

// this function builds the AST based on the given token_list
// return NULL if the token_list is NULL (wrong execution)
struct AST *build_shell_AST(struct linked_list *token_list);

#endif /* !PARSER_H */
