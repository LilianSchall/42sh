#ifndef EXPANSION_H
#define EXPANSION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "garbage_collector/garbage_collector.h"

struct var
{
    int argc;
    char **argv;
};

char *expand_var(const char *str, int quoted);

#endif /* !EXPANSION_H */

