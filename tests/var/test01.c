#include "AST/AST.h"
#include "builtin/builtin.h"
#include "linked_list/linked_list.h"
#include "token/token.h"
#include "execution/variables.h"

#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

Test(variables, var_one_var)
{
    init_variables();
    
    assign_var("test", "prout");
    cr_expect_str_eq(get_var("test"), "prout");
    
    free_variables();
}

Test(variables, var_one_var_replace)
{
    init_variables();

    assign_var("test", "prout");
    assign_var("test", "caca");

    cr_expect_str_eq(get_var("test"), "caca");

    free_variables();
}

Test(variables, var_two_var)
{
    init_variables();

    assign_var("test1", "prout");
    assign_var("test2", "caca");

    cr_expect_str_eq(get_var("test1"), "prout");
    cr_expect_str_eq(get_var("test2"), "caca");

    free_variables();
}
