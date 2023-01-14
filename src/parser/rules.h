#ifndef RULES_H
#define RULES_H

#include <stdbool.h>

#include "linked_list/linked_list.h"
#include "token/token.h"

struct AST *input_rule(struct linked_list *token_list);
struct AST *list_rule(struct linked_list *token_list);
struct AST *and_or_rule(struct linked_list *token_list, bool trigger_warn);
struct AST *pipeline_rule(struct linked_list *token_list, bool trigger_warn);
struct AST *command_rule(struct linked_list *token_list, bool trigger_warn);
struct AST *simple_command_rule(struct linked_list *token_list,
                                bool trigger_warn);
struct AST *element_rule(struct linked_list *token_list, bool trigger_warn);
struct AST *shell_command_rule(struct linked_list *token_list,
                               bool trigger_warn);
struct AST *rule_if_rule(struct linked_list *token_list, bool trigger_warn);
struct AST *else_clause_rule(struct linked_list *token_list, bool trigger_warn);
struct AST *compound_list_rule(struct linked_list *token_list,
                               bool trigger_warn);
struct AST *prefix_rule(struct linked_list *token_list, bool trigger_warn);
struct AST *redirection_rule(struct linked_list *token_list, bool trigger_warn);
struct AST *rule_while_rule(struct linked_list *token_list, bool trigger_warn);
struct AST *rule_until_rule(struct linked_list *token_list, bool trigger_warn);
struct AST *rule_for_rule(struct linked_list *token_list, bool trigger_warn);
struct AST *assignment_word_rule(struct linked_list *token_list,
                                 bool trigger_warn);

#endif /* !RULES_H */

