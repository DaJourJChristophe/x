#ifndef X_PREPROCESS_INSERTION_H
#define X_PREPROCESS_INSERTION_H

#include "token.h"

#define X_SYNTAX_TOKEN_INSERTION_STRING_MAX   128

/**
 * @brief
 */
struct syntax_token_insert
{
  const char str[X_SYNTAX_TOKEN_INSERTION_STRING_MAX];
  syntax_token_t tok;
};

/**
 * @brief
 */
typedef struct syntax_token_insert syntax_token_insert_t;

#endif/*X_PREPROCESS_INSERTION_H*/
