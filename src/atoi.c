#include "ae.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static int parse_integer(const char **s)
{
  int r = 0;
  int n = 1;
  int x;

  for (; **s; *s += 1)
  {
    if (**s != 0x20)
    {
      break;
    }
  }

  if (**s == 0)
  {
    return n * r;
  }

  if (**s == 0x2d)
  {
    n = (-1);
    *s += 1;
  }
  else if (**s == 0x2b)
  {
    n = 1;
    *s += 1;
  }

  for (; **s; *s += 1)
  {
    if (ae_match(**s, AE_NO_DIGIT))
    {
      goto done;
    }

    x = **s - 0x30;

    if ((r > (INT_MAX / 10) || (r == (INT_MAX / 10) && x >  7)) ||
        (r < (INT_MIN / 10) || (r == (INT_MIN / 10) && x < -8)))
    {
      return (n < 1) ? INT_MIN : INT_MAX;
    }

    r = 10 * r + x;
  }

done:
  return (n * r);
}

int main(void)
{
  char const data[] = "2925764";
  char const *p = data;

  printf("%d\n", parse_integer(&p));
  printf("%c\n", *p);

  return EXIT_SUCCESS;
}
