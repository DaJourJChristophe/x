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
 * @brief Check if a character is greater than or equal to 48
 *        or less than or equal to 57.
 */
AE_RESULT_T is_digit(const char c)
{
  return c >= AE_ZERO && c <= AE_NINE;
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
 * @brief Check if a character is greater than or equal to 65
 *        or less than or equal to 90.
 */
AE_RESULT_T is_upper(const char c)
{
  return c >= AE_UPPA && c <= AE_UPPZ;
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
 * @brief Check if a character is greater than or equal to 97
 *        or less than or equal to 122 or if is greater than
 *        or equal to 65 or less than or equal to 90 or if is
 *        greater than or equal to 48 or less than or equal to 57.
 */
AE_RESULT_T is_alphanum(const char c)
{
  return is_alpha(c) || is_digit(c);
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
 * @brief Given some character `c` check it matches an `expression`.
 */
bool ae_match(const char c, const char expression)
{
  switch (expression)
  {
    case AE_IS_ASCII: return  is_ascii(c);
    case AE_NO_ASCII: return !is_ascii(c);

    case AE_IS_ALPHA: return  is_alpha(c);
    case AE_NO_ALPHA: return !is_alpha(c);

    case AE_IS_DIGIT: return  is_digit(c);
    case AE_NO_DIGIT: return !is_digit(c);

    case AE_IS_LOWER: return  is_lower(c);
    case AE_NO_LOWER: return !is_lower(c);

    case AE_IS_ALNUM: return  is_alphanum(c);
    case AE_NO_ALNUM: return !is_alphanum(c);

    case AE_IS_UPPER: return  is_upper(c);
    case AE_NO_UPPER: return !is_upper(c);

    case AE_IS_WHITE: return  is_white(c);
    case AE_NO_WHITE: return !is_white(c);

    default:
      die("Unknown ASCII expression", __FILE__, __func__);
  }

  return false;
}
