#ifndef X_ASCII_EXPRESSION_H
#define X_ASCII_EXPRESSION_H

#include "common.h"

#include <stdbool.h>

/**
 * @brief A collection of positive ASCII expressions.
 */
#define AE_IS_ASCII  ((const char)0x00)
#define AE_IS_ALPHA  ((const char)0x02)
#define AE_IS_DIGIT  ((const char)0x04)
#define AE_IS_LOWER  ((const char)0x06)
#define AE_IS_ALNUM  ((const char)0x08)
#define AE_IS_UPPER  ((const char)0x0A)
#define AE_IS_WHITE  ((const char)0x0C)

/**
 * @brief A collection of negative ASCII expressions.
 */
#define AE_NO_ASCII ((const char)0x01)
#define AE_NO_ALPHA ((const char)0x03)
#define AE_NO_DIGIT ((const char)0x05)
#define AE_NO_LOWER ((const char)0x07)
#define AE_NO_ALNUM ((const char)0x09)
#define AE_NO_UPPER ((const char)0x0B)
#define AE_NO_WHITE ((const char)0x0D)

/**
 * @brief Check if a character is alphabetic.
 */
bool is_alpha(const char c);

/**
 * @brief Check if a character is alphanumeric.
 */
bool is_alphanum(const char c);

/**
 * @brief Check if a character is ASCII.
 */
bool is_ascii(const char c);

/**
 * @brief Check if a character is a digit.
 */
bool is_digit(const char c);

/**
 * @brief Check if a character is a lower case.
 */
bool is_lower(const char c);

/**
 * @brief Check if a character is a upper case.
 */
bool is_upper(const char c);

/**
 * @brief Check if a character is whitespace.
 */
bool is_white(const char c);

/**
 * @brief Check if a character matches one of the
 *        predefined supported expressions.
 */
bool ae_match(const char c, const char expression);

#endif/*X_ASCII_EXPRESSION_H*/
