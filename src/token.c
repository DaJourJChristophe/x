#include "common.h"
#include "token.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/**
 * @brief Allocate a new syntax token and set both the type and the data;
 */
syntax_token_t *syntax_token_new(int type, void *data, size_t const n)
{
  syntax_token_t *token = NULL;
  token = __calloc(1, sizeof(syntax_token_t));

  token->type = type;
  token->data = __malloc(n);

  memcpy(token->data, data, n);
  return token;
}

/**
 * @brief Convert the syntax token data structure to bytes.
 */
uint8_t *syntax_token_to_bytes(syntax_token_t *token, size_t *n)
{
  if (token == NULL)
  {
    *n = 0;
    return NULL;
  }

  uint8_t *buffer = NULL;
  *n = sizeof(int) + sizeof(size_t) + token->size;
  buffer = __calloc(*n, sizeof(uint8_t));

  memcpy( buffer,                                 &token->type, sizeof(int));
  memcpy((buffer + sizeof(int)),                  &token->size, sizeof(size_t));

  if (token->data != NULL)
  {
    memcpy((buffer + sizeof(int) + sizeof(size_t)),  token->data, token->size);
  }

  return buffer;
}

/**
 * @brief Convert the bytes to syntax token data structure.
 */
syntax_token_t *syntax_token_from_bytes(uint8_t *buffer)
{
  syntax_token_t *token = NULL;
  token = __calloc(1, sizeof(syntax_token_t));

  token->type = *(int *)buffer;
  buffer += sizeof(int);

  token->size = *(size_t *)buffer;
  buffer += sizeof(size_t);

  // memcpy(&token->type,  buffer,                sizeof(int));
  // memcpy(&token->size, (buffer + sizeof(int)), sizeof(size_t));

  if (token->size > 0)
  {
    token->data = __calloc(token->size, sizeof(uint8_t));
    memcpy(token->data, buffer, token->size);
  }

  return token;
}

/**
 * @brief Destroy the token and the token data.
 */
void syntax_token_destroy(syntax_token_t *token)
{
  if (token != NULL)
  {
    __free(token->data);
  }
  __free(token);
}
