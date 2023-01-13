#include "parser.h"

struct AST *pipeline_rule(struct linked_list *token_list, bool trigger_warn)
{
    return command_rule(token_list, trigger_warn);
}
