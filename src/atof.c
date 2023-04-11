#include "ae.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void parse_float(const char **s, float *y)
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
    *y = n * r;
    return;
  }

  if (**s == 0x2d)
  {
    n = (-1);
    *s+=1;
  }
  else if (**s == 0x2b)
  {
    n = 1;
    *s+=1;
  }

  for (; **s && **s != '.'; *s+=1)
  {
    if (ae_match(**s, AE_NO_DIGIT))
    {
      break;
    }

    x = **s - 0x30;

    if ((r > (INT_MAX / 10) || (r == (INT_MAX / 10) && x >  7)) ||
        (r < (INT_MIN / 10) || (r == (INT_MIN / 10) && x < -8)))
    {
      *y = (n < 1) ? INT_MIN : INT_MAX;
      return;
    }

    r = 10 * r + x;
  }

  *y = n * r;

  if (**s == '.')
  {
    *s+=1;
  }

  r = 0;
  n = 1;

  for (; **s; *s+=1)
  {
    if (**s != 0x20)
    {
      break;
    }
  }

  if (**s == 0)
  {
    float f = n * r;
    while (f >= 1)
    {
      f /= 10;
    }
    *y += f;
    return;
  }

  if (**s == 0x2d)
  {
    n = (-1);
    *s+=1;
  }
  else if (**s == 0x2b)
  {
    n = 1;
    *s+=1;
  }

  for (; **s; *s+=1)
  {
    if (ae_match(**s, AE_NO_DIGIT))
    {
      break;
    }

    x = **s - 0x30;

    if ((r > (INT_MAX / 10) || (r == (INT_MAX / 10) && x >  7)) ||
        (r < (INT_MIN / 10) || (r == (INT_MIN / 10) && x < -8)))
    {
      float f = (n < 1) ? INT_MIN : INT_MAX;
      while (f >= 1)
      {
        f /= 10;
      }
      *y += f;
      return;
    }

    r = 10 * r + x;
  }

  float f = n * r;
  while (f >= 1)
  {
    f /= 10;
  }
  *y += f;
}

int main(void)
{
  char const data[] = "3.4";
  char const *p = data;

  parse_float(p);

  return EXIT_SUCCESS;
}
