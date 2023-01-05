#ifndef EXEC_ECHO_H
#define EXEC_ECHO_H

#include <stdio.h>
#include <stddef.h>

#include "AST/AST.h"
#include "token/token.h"
#include "linked_list/linked_list.h"
#include "execution/execution.h"

// struct to stock echo option
struct echo_option {
    char option_n; // -n inhibits printing a newline
    char option_E; // -e enable the interpretation of \n, \t and \\ escapes
    char option_e; // -E disable the interpretation of \n, \t and \\ escapes.
};

// execute the command 'echo' on the given AST
// return 0 is command is successfuly executate, else 2
int exec_echo(struct AST *tree);

#endif /* !EXEC_ECHO_H */