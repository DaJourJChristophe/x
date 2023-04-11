#include "ae.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void parse_float(const char *s)
{
  int r = 0;
  int n = 1;
  int x;
  float y;

  for (; *s; s++)
  {
    if (*s != 0x20)
    {
      break;
    }
  }

  if (*s == 0)
  {
    y = n * r;
    printf("%f\n", y);
    return;
  }

  if (*s == 0x2d)
  {
    n = (-1);
    s++;
  }
  else if (*s == 0x2b)
  {
    n = 1;
    s++;
  }

  for (; *s && *s != '.'; s++)
  {
    if (ae_match(*s, AE_NO_DIGIT))
    {
      break;
    }

    x = *s - 0x30;

    if ((r > (INT_MAX / 10) || (r == (INT_MAX / 10) && x >  7)) ||
        (r < (INT_MIN / 10) || (r == (INT_MIN / 10) && x < -8)))
    {
      y = (n < 1) ? INT_MIN : INT_MAX;
      printf("%f\n", y);
      return;
    }

    r = 10 * r + x;
  }

  y = n * r;

  if (*s == '.')
  {
    s++;
  }

  r = 0;
  n = 1;

  for (; *s; s++)
  {
    if (*s != 0x20)
    {
      break;
    }
  }

  if (*s == 0)
  {
    y += 1 / (n * r);
    printf("%f\n", y);
    return;
  }

  if (*s == 0x2d)
  {
    n = (-1);
    s++;
  }
  else if (*s == 0x2b)
  {
    n = 1;
    s++;
  }

  for (; *s; s++)
  {
    if (ae_match(*s, AE_NO_DIGIT))
    {
      break;
    }

    x = *s - 0x30;

    if ((r > (INT_MAX / 10) || (r == (INT_MAX / 10) && x >  7)) ||
        (r < (INT_MIN / 10) || (r == (INT_MIN / 10) && x < -8)))
    {
      y += 1 / (float)((n < 1) ? INT_MIN : INT_MAX);
      printf("%f\n", y);
      return;
    }

    r = 10 * r + x;
  }

  y += (float)(1 / (n * r));
  printf("%f\n", y);
}

int main(void)
{
  char const data[] = "3.4";
  char const *p = data;

  parse_float(p);

  return EXIT_SUCCESS;
}
