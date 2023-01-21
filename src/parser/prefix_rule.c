#include "parser.h"

struct AST *prefix_rule(struct linked_list *token_list, bool trigger_warn)
{
    struct token *token = list_head(token_list);

    if (token->type == VARASSIGNMENT)
        return assignment_word_rule(token_list, trigger_warn);
    else if (token->type == IO_NUMBER || is_redirect(token))
        return redirection_rule(token_list, trigger_warn);
    else
        return NULL;
}
