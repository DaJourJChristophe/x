#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include "cmocka.h"

#include "common.h"
#include "ae.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef bool (*all_chars_t)(const char);

static void enum_all_chars_option(all_chars_t fn,
  const int a, const int b, const int d, const int e)
{
  const int n = 128;
  char c = -127;

  for (int i = -127; i < n; i++, c++)
  {
    if (i == a || i == b || i == d || i == e)
    {
      assert_true(fn(c));
    }
    else
    {
      assert_false(fn(c));
    }
  }
}

static void enum_all_chars(all_chars_t fn,
  const int l_bnd, const int u_bnd)
{
  const int n = 128;
  char c = -127;

  for (int i = -127; i < n; i++, c++)
  {
    if (i >= l_bnd && i <= u_bnd)
    {
      assert_true(fn(c));
    }
    else
    {
      assert_false(fn(c));
    }
  }
}

static void enum_all_chars_double_bound(all_chars_t fn,
  const int l_bnd_1, const int u_bnd_1,
  const int l_bnd_2, const int u_bnd_2)
{
  const int n = 128;
  char c = -127;

  for (int i = -127; i < n; i++, c++)
  {
    if ((i >= l_bnd_1 && i <= u_bnd_1) ||
        (i >= l_bnd_2 && i <= u_bnd_2))
    {
      assert_true(fn(c));
    }
    else
    {
      assert_false(fn(c));
    }
  }
}

static void enum_all_chars_multi_bound(all_chars_t fn,
  const int a, const int b, const int d, const int e,
  const int f, const int g)
{
  const int n = 128;
  char c = -127;

  for (int i = -127; i < n; i++, c++)
  {
    if ((i >= a && i <= b) ||
        (i >= d && i <= e) ||
        (i >= f && i <= g))
    {
      assert_true(fn(c));
    }
    else
    {
      assert_false(fn(c));
    }
  }
}

static void is_digit_test(void unused **state)
{
  enum_all_chars(&is_digit, 48, 57);
}

static void is_lower_test(void unused **state)
{
  enum_all_chars(&is_lower, 97, 122);
}

static void is_upper_test(void unused **state)
{
  enum_all_chars(&is_upper, 65, 90);
}

static void is_alpha_test(void unused **state)
{
  enum_all_chars_double_bound(&is_alpha, 65, 90, 97, 122);
}

static void is_ascii_test(void unused **state)
{
  enum_all_chars(&is_ascii, 0, 127);
}

static void is_alphanum_test(void unused **state)
{
  enum_all_chars_multi_bound(&is_alphanum, 65, 90, 97, 122, 48, 57);
}

static void is_white_test(void unused **state)
{
  enum_all_chars_option(&is_white, 9, 10, 13, 32);
}

static void ae_match_test(void unused **state)
{
  const int expressions[] = {
    AE_IS_ASCII,
    AE_NO_ASCII,
    AE_IS_ALPHA,
    AE_NO_ALPHA,
    AE_IS_DIGIT,
    AE_NO_DIGIT,
    AE_IS_LOWER,
    AE_NO_LOWER,
    AE_IS_ALNUM,
    AE_NO_ALNUM,
    AE_IS_UPPER,
    AE_NO_UPPER,
    AE_IS_WHITE,
    AE_NO_WHITE,
  };
  const int expressions_count = sizeof(expressions) / sizeof(expressions[0]);

  const int n = 128;
  char c = -127;
  int j = -127;
  int i;

  bool result;

  for (; j < n; j++, c++)
  {
    for (i = 0; i < expressions_count; i++)
    {
      if (i % 2 == 0)
      {
        switch (expressions[i])
        {
          case AE_IS_ASCII:
            result = ae_match(c, AE_IS_ASCII);
            if (c >= 0)
            {
              assert_true(result);
            }
            else
            {
              assert_false(result);
            }
            break;
          case AE_IS_ALPHA:
            result = ae_match(c, AE_IS_ALPHA);
            if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
            {
              assert_true(result);
            }
            else
            {
              assert_false(result);
            }
            break;
          case AE_IS_DIGIT:
            result = ae_match(c, AE_IS_DIGIT);
            if ((c >= 48 && c <= 57))
            {
              assert_true(result);
            }
            else
            {
              assert_false(result);
            }
            break;
          case AE_IS_LOWER:
            result = ae_match(c, AE_IS_LOWER);
            if ((c >= 97 && c <= 122))
            {
              assert_true(result);
            }
            else
            {
              assert_false(result);
            }
            break;
          case AE_IS_ALNUM:
            result = ae_match(c, AE_IS_ALNUM);
            if ((c >= 65 && c <= 90)  ||
                (c >= 97 && c <= 122) ||
                (c >= 48 && c <= 57))
            {
              assert_true(result);
            }
            else
            {
              assert_false(result);
            }
            break;
          case AE_IS_UPPER:
            result = ae_match(c, AE_IS_UPPER);
            if ((c >= 65 && c <= 90))
            {
              assert_true(result);
            }
            else
            {
              assert_false(result);
            }
            break;
          case AE_IS_WHITE:
            result = ae_match(c, AE_IS_WHITE);
            if (c == 9 || c == 10 || c == 13 || c == 32)
            {
              assert_true(result);
            }
            else
            {
              assert_false(result);
            }
            break;
        }
      }
      else
      {
        switch (expressions[i])
        {
          case AE_NO_ASCII:
            result = ae_match(c, AE_NO_ASCII);
            if (c >= 0)
            {
              assert_false(result);
            }
            else
            {
              assert_true(result);
            }
            break;
          case AE_NO_ALPHA:
            result = ae_match(c, AE_NO_ALPHA);
            if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
            {
              assert_false(result);
            }
            else
            {
              assert_true(result);
            }
            break;
          case AE_NO_DIGIT:
            result = ae_match(c, AE_NO_DIGIT);
            if ((c >= 48 && c <= 57))
            {
              assert_false(result);
            }
            else
            {
              assert_true(result);
            }
            break;
          case AE_NO_LOWER:
            result = ae_match(c, AE_NO_LOWER);
            if ((c >= 97 && c <= 122))
            {
              assert_false(result);
            }
            else
            {
              assert_true(result);
            }
            break;
          case AE_NO_ALNUM:
            result = ae_match(c, AE_NO_ALNUM);
            if ((c >= 65 && c <= 90)  ||
                (c >= 97 && c <= 122) ||
                (c >= 48 && c <= 57))
            {
              assert_false(result);
            }
            else
            {
              assert_true(result);
            }
            break;
          case AE_NO_UPPER:
            result = ae_match(c, AE_NO_UPPER);
            if ((c >= 65 && c <= 90))
            {
              assert_false(result);
            }
            else
            {
              assert_true(result);
            }
            break;
          case AE_NO_WHITE:
            result = ae_match(c, AE_NO_WHITE);
            if (c == 9 || c == 10 || c == 13 || c == 32)
            {
              assert_false(result);
            }
            else
            {
              assert_true(result);
            }
            break;
        }
      }
    }
  }
}

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(is_digit_test),
    cmocka_unit_test(is_lower_test),
    cmocka_unit_test(is_upper_test),
    cmocka_unit_test(is_alpha_test),
    cmocka_unit_test(is_ascii_test),
    cmocka_unit_test(is_white_test),
    cmocka_unit_test(is_alphanum_test),

    cmocka_unit_test(ae_match_test),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
