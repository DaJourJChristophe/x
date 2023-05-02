#ifndef X_TOKEN_H
#define X_TOKEN_H

#include <stddef.h>
#include <stdint.h>

struct token
{
   int   type;      /* Token datatype declaration. */
  void  *data;      /* Data associated with the corresponding datatype. */
  size_t size;
};

/**
 * @brief Define a namespace for the token structure.
 */
typedef struct token token_t;

/**
 * @brief Define a namespace for the syntax token structure.
 */
typedef token_t syntax_token_t;

/**
 * @brief Allocate a new syntax token and set both the type and the data;
 */
syntax_token_t *syntax_token_new(int type, void *data, size_t const n);

/**
 * @brief Convert the syntax token data structure to bytes.
 */
uint8_t *syntax_token_to_bytes(syntax_token_t *token, size_t *n);

/**
 * @brief Convert the bytes to syntax token data structure.
 */
syntax_token_t *syntax_token_from_bytes(uint8_t *buffer);

/**
 * @brief Destroy the token and the token data.
 */
void syntax_token_destroy(syntax_token_t *token);

#endif/*X_TOKEN_H*/
