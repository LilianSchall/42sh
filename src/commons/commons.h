#ifndef COMMONS_H
#define COMMONS_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AST/AST.h"
#include "evalexpr/evalexpr.h"
#include "execution/execution.h"
#include "expansion/expansion.h"
#include "garbage_collector/garbage_collector.h"
#include "linked_list/linked_list.h"

#define SINGLE_QUOTE_MARKER -3
#define DOUBLE_QUOTE_MARKER -2
#define DELIMITER_MARKER -1

extern struct status *status;

struct status
{
    int continue_val;
    int break_val;
    int nb_loop;
    int exit_code;
    int exit_bool;
};

// create the status structure
void new_status(void);

// free the status structure
void free_status(void);

// return true if we need to continue in boucle
// return false if we need to stop
int check_status(void);

void increase_nb_loop(void);

void decrease_nb_loop(void);

// this function duplicates the string str into a newly allocated
// string. It will delete every byte that represent a negative nb by nospace
int my_strdup(char *str, struct symbol *sym);

// search if the given str is in special tokens and returns its index
// returns -1 if it hasn't found any
int find_special_tokens(char *str, char **special_tokens);

// search if the given str is a substring of a special token
// and returns its index: returns -1 if it hasn't found any
int sub_special_tokens(char *str, char **special_tokens);

// search if the given char c is a delimitator and returns its index
// returns -1 if it hasn't found any
int find_delims(char c, char *delims);

// replace all occurence of 'occ' by 'c' in 'string'
char *str_replace(char *string, char *occ, char *c);

// allocate and copy the string into a new char *
char *copy_string(char *src);

// transform a linked list into a argc (char **)
// argc is a pointer
// last elem of argv is NULL
char **new_argv(struct AST *tree, int *argc, struct env *env);

// free argv
void free_argv(char **argv);

// check if the string is a number
// if yes, it returns its value
// if not, it returns -1
int my_itoa(char *string);

// check if char c is a filed spliter
int is_ifs(char c);

#endif /* !COMMONS_H */
