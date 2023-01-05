#include <err.h>
#include <stdio.h>

#include "kernel/kernel.h"
#include "option_parser/option_parser.h"

int main(int argc, char **argv)
{
#if 0
    char *filename = NULL;
    int options = parse_given_args(argc, argv, &filename);

    if (options == -1 || 
        (filename && 
        !is_option_activated(options, SCRIPT_READING))
        errx(1, "Usage: ./42sh [options] [script]");
    
    return options;
#endif
    
    char input[] = "echo foo;";
    struct linked_list *token_list = build_token_list(input);
    for (struct linked_node *node = token_list->head; node; node = node->next)
    {
        struct token *token = node->data;   
        printf("sym: %s\n",token->symbol);
        printf("type: %d\n", token->type);
    }
    deep_free_list(token_list, free_token);

    return 0;
}
