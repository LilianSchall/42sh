#ifndef COMMONS_H
#define COMMONS_H

#include <stdlib.h>
#include <string.h>

// this function duplicates the string str into a newly allocated
// string. It will delete every byte that represent -1 by nospace
char *my_strdup(char *str);

// search if the given str is in special tokens and returns its index
// returns -1 if it hasn't found any
int find_special_tokens(char *str, char **special_tokens);

// search if the given char c is a delimitator and returns its index
// returns -1 if it hasn't found any
int find_delims(char c , char *delims);

#endif /* !COMMONS_H */
