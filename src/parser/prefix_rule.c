#include "parser.h"

struct AST *prefix_rule(struct linked_list *token_list, bool trigger_warn)
{
    return redirection_rule(token_list, trigger_warn);
}
