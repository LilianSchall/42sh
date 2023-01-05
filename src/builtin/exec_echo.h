#ifndef EXEC_ECHO_H
#define EXEC_ECHO_H

#include <stdio.h>
#include <stddef.h>
#include <string.h>


#include "AST/AST.h"
#include "token/token.h"
#include "linked_list/linked_list.h"
#include "execution/execution.h"
#include "commons/commons.h"

// struct to stock echo option
struct echo_option {
    char option_n; // -n inhibits printing a newline
    char option_E; // -e enable the interpretation of \n, \t and \\ escapes
    char option_e; // -E disable the interpretation of \n, \t and \\ escapes.
};

// execute the command 'echo' on the given AST
// return 0 is command is successfuly executate, else 2
int echo_fn(struct AST *tree);

// return a linked list of all 'word' in our AST 
// it only takes the left child
// example of usage : get echo words
struct linked_list *get_argv_signe_AST(struct AST *new_AST);

#endif /* !EXEC_ECHO_H */