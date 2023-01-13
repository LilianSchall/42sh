#include <err.h>
#include <stdio.h>
#include <criterion/criterion.h>

#include "kernel/kernel.h"
#include "option_parser/option_parser.h"
#include "expansion/expansion.h"

Test(expansion, one_var)
{
    // Test 1: Check for basic expansion
    setenv("NAME", "Lilian", 1);
    char *str = "Hello $NAME";
    char *result = expand_var(str);
    cr_expect_str_eq("Hello Lilian", result);
    free(result);
}

Test(expansion, two_var)
{
    // Test 2: Check for multiple expansions
    setenv("NAME", "Lili", 1);
    setenv("NAME1", "Entinque", 1);
    char *str = "Hello $NAME and $NAME1";
    char *result = expand_var(str);
    cr_expect_str_eq("Hello Lili and Entinque", result);
    free(result);
}

Test(expansion, one_var_non_exist)
{
    // Test 3: Check for not existing var
    char *str = "Hello $NOTEXIST";
    char *result = expand_var(str);
    cr_expect_str_eq("Hello ", result);
    free(result);
}
