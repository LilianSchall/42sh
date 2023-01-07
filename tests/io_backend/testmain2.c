#include "io_backend/io_backend.h"

int main(void)
{
    char *str = get_interactive_content();
    if (strcmp(str, "Salut je suis le testeur comment ca va ?\n fffwefefwefwefwe\n1548!@##&$*&\nbyebye\n") == 0)
       printf("yes\n");
    else
        printf("no\n");
    return 0;
}
