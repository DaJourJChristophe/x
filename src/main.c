#include "ae.h"

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  const char c = '3';

  printf("result: %u\n", is_digit(c));

  return EXIT_SUCCESS;
}
