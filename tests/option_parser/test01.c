#include <criterion/criterion.h>

#include "option_parser/option_parser.h"

Test(option_parser, test_given_file)
{
    int argc = 2;
    char *argv[] = { "42sh", "file.sh", };

    char *filename = NULL;
    char *input = NULL;

    int result = parse_given_args(argc, argv, &filename, &input);

    cr_assert_eq(result, 1);
    cr_assert_str_eq(filename, "file.sh");
}
