#ifndef PARSING_TOOLS_H
#define PARSING_TOOLS_H

#include <err.h>
#include <stdbool.h>
#include <string.h>

#include "AST/AST.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

struct log_info
{
    char *sym;
    char *rulename;
    bool trigger_warn;
};

struct token *consume_token(struct linked_list *token_list,
                            enum token_type type, struct log_info info);

struct AST *combine_token_AST(struct AST *tree, struct token *token);

#endif /* !PARSING_TOOLS_H */
