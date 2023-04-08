#ifndef X_ASCII_EXPRESSION_H
#define X_ASCII_EXPRESSION_H

#ifdef __cplusplus
extern "C"{
#endif

#include "common.h"

#include <stdbool.h>

/**
 * @brief A collection of positive ASCII expressions.
 */
#define AE_IS_ASCII  ((const int)0x00)
#define AE_IS_ALPHA  ((const int)0x02)
#define AE_IS_DIGIT  ((const int)0x04)
#define AE_IS_LOWER  ((const int)0x06)
#define AE_IS_ALNUM  ((const int)0x08)
#define AE_IS_UPPER  ((const int)0x0A)
#define AE_IS_WHITE  ((const int)0x0C)

/**
 * @brief A collection of negative ASCII expressions.
 */
#define AE_NO_ASCII ((const int)0x01)
#define AE_NO_ALPHA ((const int)0x03)
#define AE_NO_DIGIT ((const int)0x05)
#define AE_NO_LOWER ((const int)0x07)
#define AE_NO_ALNUM ((const int)0x09)
#define AE_NO_UPPER ((const int)0x0B)
#define AE_NO_WHITE ((const int)0x0D)

/**
 * @brief Check if a character is alphabetic.
 */
bool is_alpha(const char c);

/**
 * @brief Check if a character is not alphabetic.
 */
bool no_alpha(const char c);

/**
 * @brief Check if a character is alphanumeric.
 */
bool is_alnum(const char c);

/**
 * @brief Check if a character is not alphanumeric.
 */
bool no_alnum(const char c);

/**
 * @brief Check if a character is ASCII.
 */
bool is_ascii(const char c);

/**
 * @brief Check if a character is not ASCII.
 */
bool no_ascii(const char c);

/**
 * @brief Check if a character is a digit.
 */
bool is_digit(const char c);

/**
 * @brief Check if a character is not a digit.
 */
bool no_digit(const char c);

/**
 * @brief Check if a character is lower case.
 */
bool is_lower(const char c);

/**
 * @brief Check if a character is not lower case.
 */
bool no_lower(const char c);

/**
 * @brief Check if a character is upper case.
 */
bool is_upper(const char c);

/**
 * @brief Check if a character is not upper case.
 */
bool no_upper(const char c);

/**
 * @brief Check if a character is whitespace.
 */
bool is_white(const char c);

/**
 * @brief Check if a character is not whitespace.
 */
bool no_white(const char c);

/**
 * @brief Check if a character matches one of the
 *        predefined supported expressions.
 */
bool ae_match(const char c, const int expression);

#ifdef __cplusplus
}
#endif

#endif/*X_ASCII_EXPRESSION_H*/
