#include <criterion/criterion.h>
#include "io_backend/io_backend.h"


Test(io_backend, test_get_file_content_1)
{
    char *str = get_file_content("file_to_get_content_01");
    cr_assert_str_eq(str, "Salut je suis le testeur comment ca va ?\n fffwefefwefwefwe\n1548!@##&$*&\nbyebye\n");
}

