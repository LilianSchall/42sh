#ifndef COMMONS_H
#define COMMONS_H

#include <stdlib.h>
#include <string.h>

#include "linked_list/linked_list.h"
#include "AST/AST.h"

// this function duplicates the string str into a newly allocated
// string. It will delete every byte that represent -1 by nospace
char *my_strdup(char *str);

// search if the given str is in special tokens and returns its index
// returns -1 if it hasn't found any
int find_special_tokens(char *str, char **special_tokens);

// search if the given char c is a delimitator and returns its index
// returns -1 if it hasn't found any
int find_delims(char c , char *delims);

// replace all occurence of 'occ' by 'c' in 'string'
char *str_replace(char* string, char* occ, char* c);

// allocate and copy the string into a new char *
char *copy_string(char *src);

// transform a linked list into a argc (char **)
// argc is a pointer
char **new_argv(struct linked_list *linked_list, int *argc);

// free argv
void free_argv(int argc, char **argv);

// return a linked list of all 'word' in our AST 
// it only takes the left child
// example of usage : get echo words
struct linked_list *get_linked_list_from_AST(struct AST *AST);

#endif /* !COMMONS_H */