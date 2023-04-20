#include "common.h"
#include "token.h"

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
 * @brief Destroy the token and the token data.
 */
void syntax_token_destroy(syntax_token_t *token)
{
  __free(token->data);
  __free(token);
}
