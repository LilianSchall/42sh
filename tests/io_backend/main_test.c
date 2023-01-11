#include "io_backend.h"

int main(void)
{
  char *str = get_interactive_content(true);
  printf("%s", str);
  return 0;
}
