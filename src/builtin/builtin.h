#ifndef BUILTIN_H
#define BUILTIN_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "AST/AST.h"
#include "commons/commons.h"
#include "execution/execution.h"
#include "linked_list/linked_list.h"
#include "token/token.h"

// struct to stock echo option
struct echo_option
{
    char option_n; // -n inhibits printing a newline
    char option_E; // -e enable the interpretation of \n, \t and \\ escapes
    char option_e; // -E disable the interpretation of \n, \t and \\ escapes.
};

// echo  builtin function
// take argc & argv in parameter
int echo_fn(int argc, char **argv);

// return 0
int true_fn(int argc, char **argv);

// return 1
int false_fn(int argc, char **argv);

#endif /* !BUILTIN_H */
