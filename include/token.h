#ifndef X_TOKEN_H
#define X_TOKEN_H

#include <stddef.h>

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

#endif/*X_TOKEN_H*/
