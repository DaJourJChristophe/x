/**
 * @file ae.c
 * @author Da'Jour J. Christophe (dajour.christophe@gmail.com)
 * @brief Define simple character regular expressions.
 * @version 0.1
 * @date 2023-04-08
 *
 * @copyright Copyright (c) 2023 Da'Jour J. Christophe. All rights reserved.
 */
#include "common.h"
#include "ae.h"

/*
NOTE: Change is_white to is_space.
*/

/**
 * @brief A collection of ASCII expression evaluation characters.
 */
#define AE_NULL   ((char)0x00)  /* null */
#define AE_ZERO   ((char)0x30)  /* zero */
#define AE_NINE   ((char)0x39)  /* nine */
#define AE_LOWA   ((char)0x61)  /* lower case a */
#define AE_LOWZ   ((char)0x7A)  /* lower case z */
#define AE_UPPA   ((char)0x41)  /* upper case a */
#define AE_UPPZ   ((char)0x5A)  /* upper case z */
#define AE_TAB    ((char)0x09)  /* tab */
#define AE_BREAK  ((char)0x0A)  /* line-break */
#define AE_SPACE  ((char)0x20)  /* space */
#define AE_RETURN ((char)0x0D)  /* carriage return */
#define AE_FORM_FEED ((char)0x0C)  /* form feed */
#define AE_VERTICAL_TAB ((char)0x0B)  /* vertical tab */

/**
 * @brief An inline boolean return type.
 */
#define AE_RESULT_T inline bool always_inline

/**
 * @brief Check if a character is greater than or equal to zero.
 */
AE_RESULT_T is_ascii(const char c)
{
  return c >= AE_NULL;
}

/**
 * @brief Check if a character is not greater than or equal to zero.
 */
AE_RESULT_T no_ascii(const char c)
{
  return c < AE_NULL;
}

/**
 * @brief Check if a character is greater than or equal to 48
 *        or less than or equal to 57.
 */
AE_RESULT_T is_digit(const char c)
{
  return c >= AE_ZERO && c <= AE_NINE;
}

/**
 * @brief Check if a character is not greater than or not equal to 48
 *        or not less than or not equal to 57.
 */
AE_RESULT_T no_digit(const char c)
{
  return c < AE_ZERO || c > AE_NINE;
}

/**
 * @brief Check if a character is greater than or equal to 97
 *        or less than or equal to 122.
 */
AE_RESULT_T is_lower(const char c)
{
  return c >= AE_LOWA && c <= AE_LOWZ;
}

/**
 * @brief Check if a character is not greater than or not equal to 97
 *        or not less than or not equal to 122.
 */
AE_RESULT_T no_lower(const char c)
{
  return c < AE_LOWA || c > AE_LOWZ;
}

/**
 * @brief Check if a character is greater than or equal to 65
 *        or less than or equal to 90.
 */
AE_RESULT_T is_upper(const char c)
{
  return c >= AE_UPPA && c <= AE_UPPZ;
}

/**
 * @brief Check if a character is not greater than or not equal to 65
 *        or not less than or not equal to 90.
 */
AE_RESULT_T no_upper(const char c)
{
  return c < AE_UPPA || c > AE_UPPZ;
}

/**
 * @brief Check if a character is greater than or equal to 97
 *        or less than or equal to 122 or if is greater than
 *        or equal to 65 or less than or equal to 90.
 */
AE_RESULT_T is_alpha(const char c)
{
  return is_lower(c) || is_upper(c);
}

/**
 * @brief Check if a character is not greater than or not equal to 97
 *        or not less than or not equal to 122 or if is not greater than
 *        or not equal to 65 or not less than or not equal to 90.
 */
AE_RESULT_T no_alpha(const char c)
{
  return no_lower(c) && no_upper(c);
}

/**
 * @brief Check if a character is greater than or equal to 97
 *        or less than or equal to 122 or if is greater than
 *        or equal to 65 or less than or equal to 90 or if is
 *        greater than or equal to 48 or less than or equal to 57.
 */
AE_RESULT_T is_alnum(const char c)
{
  return is_alpha(c) || is_digit(c);
}

/**
 * @brief Check if a character is not greater than or not equal to 97
 *        or not less than or not equal to 122 or if is not greater than
 *        or not equal to 65 or less than or not equal to 90 or if is
 *        not greater than or not equal to 48 or not less than or not equal to 57.
 */
AE_RESULT_T no_alnum(const char c)
{
  return no_alpha(c) && no_digit(c);
}

/**
 * @brief Check if a character is equal to 9 or equal to 10 or
 *        equal to 13 or equal to 32.
 */
AE_RESULT_T is_white(const char c)
{
  return  c == AE_TAB    ||
          c == AE_BREAK  ||
          c == AE_RETURN ||
          c == AE_SPACE;
}

/**
 * @brief Check if a character is not equal to 9 or not equal to 10 or
 *        not equal to 13 or not equal to 32.
 */
AE_RESULT_T no_white(const char c)
{
  return  c != AE_TAB    &&
          c != AE_BREAK  &&
          c != AE_RETURN &&
          c != AE_SPACE;
}

/**
 * @brief Simplify the expression function definition.
 */
typedef bool (*ae_expression_t)(const char);

/**
 * @brief A collection of expression function definition to speedup
 *        how expressions are chosen when using `ae_match`.
 */
static const ae_expression_t expressions[] = {
  &is_ascii,
  &no_ascii,
  &is_alpha,
  &no_alpha,
  &is_digit,
  &no_digit,
  &is_lower,
  &no_lower,
  &is_alnum,
  &no_alnum,
  &is_upper,
  &no_upper,
  &is_white,
  &no_white,
};

/**
 * @brief Given some character `c` check it matches an `expression`.
 */
bool ae_match(const char c, const int expression)
{
  /* Check if the parameter `expression` is in the range of the
     expression function pool above, if not throw an error. */
  if (expression < 0 || expression > 14)
  {
    const char errmsg[] = "Unknown ASCII expression";
    die(errmsg, __FILE__, __func__);
    return false;
  }
  /* Select an expression function and store as an `ae_expression_t`. */
  ae_expression_t const fn = expressions[expression];
  /* Execute the expression and return to result to the caller. */
  return fn(c);
}
