#include <criterion/criterion.h>

#include "option_parser/option_parser.h"

Test(option_parser, test_given_file)
{
    int argc = 3;
    char *argv[] = { "42sh", "-c", "file.sh", };

    char *filename = NULL;

    int result = parse_given_args(argc, argv, &filename);

    cr_assert_eq(result, 1);
    cr_assert_str_eq(filename, "file.sh");
}
