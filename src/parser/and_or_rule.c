#include "parser.h"

struct AST *and_or_rule(struct linked_list *token_list, bool trigger_warn)
{
    return pipeline_rule(token_list, trigger_warn);
}
