#include "io_backend/io_backend.h"



int main(void)
{
    char *str = get_file_content("file_to_get_content_01");
    if (strcmp(str, "Salut je suis le testeur comment ca va ? fffwefefwefwefwe1548!@##&$*&byebye") ==0)
       printf("yes\n");
    else
        printf("no\n");
    return 0;
}
