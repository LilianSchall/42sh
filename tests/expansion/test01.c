#include <err.h>
#include <stdio.h>
#include <criterion/criterion.h>

#include "kernel/kernel.h"
#include "option_parser/option_parser.h"
#include "expansion/expansion.h"

Test(expansion, one_var)
{
    // Test 1: Check for basic expansion
    setenv("NAME", "Lili", 1);
    char *str = strdup("Hello $NAME");
    expand_var(&str);
    cr_expect_str_eq("Hello Lili", str);
    free(str);
}

Test(expansion, two_var)
{
    // Test 2: Check for multiple expansions
    setenv("NAME", "Lili", 1);
    setenv("NAME1", "Entinque", 1);
    char *str = strdup("Hello $NAME and $NAME1");
    expand_var(&str);
    cr_expect_str_eq("Hello Lili and Entinque", str);
    free(str);
}

Test(expansion, one_var_non_exist)
{
    char *str = strdup("Hello $NAME");
    expand_var(&str);
    cr_expect_str_eq("Hello ", str);
    free(str);
}
