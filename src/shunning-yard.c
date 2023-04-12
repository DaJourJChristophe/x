#include "queue.h"

#include <ctype.h>
#include <stdlib.h>

int main(void)
{
  const char data[] = "5 + 5";
  const char *p = data;

  for (; *p; p++)
  {
    if (isdigit(*p))
    {

    }
  }

  return EXIT_SUCCESS;
}
