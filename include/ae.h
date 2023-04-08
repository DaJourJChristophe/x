#ifndef X_ASCII_EXPRESSION_H
#define X_ASCII_EXPRESSION_H

#include "common.h"

#include <stdbool.h>

/**
 * @brief An inline boolean return type.
 */
#define AE_RESULT_T inline bool always_inline

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

#endif/*X_ASCII_EXPRESSION_H*/
