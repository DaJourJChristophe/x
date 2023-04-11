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

typedef bool (*all_chars_t)(const int);
typedef bool (*all_chars_char_t)(const char);

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

static void enum_all_chars_char(all_chars_char_t fn,
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
  enum_all_chars(&_ae_is_digit, 48, 57);
}

static void is_lower_test(void unused **state)
{
  enum_all_chars(&_ae_is_lower, 97, 122);
}

static void is_upper_test(void unused **state)
{
  enum_all_chars(&_ae_is_upper, 65, 90);
}

static void is_alpha_test(void unused **state)
{
  enum_all_chars_double_bound(&_ae_is_alpha, 65, 90, 97, 122);
}

static void is_ascii_test(void unused **state)
{
  enum_all_chars_char(&_ae_is_ascii, 0, 127);
}

static void is_alnum_test(void unused **state)
{
  enum_all_chars_multi_bound(&_ae_is_alnum, 65, 90, 97, 122, 48, 57);
}

static void is_white_test(void unused **state)
{
  enum_all_chars_option(&_ae_is_space, 9, 10, 13, 32);
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
    cmocka_unit_test(is_alnum_test),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
