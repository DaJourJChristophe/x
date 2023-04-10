#ifndef X_LEXER_H
#define X_LEXER_H

#ifdef __cplusplus
extern "C"{
#endif

struct token
{
   int  type;      /* Token datatype declaration. */
  void *data;      /* Data associated with the corresponding datatype. */

  /**
   * NOTE: Potentially will not apply once data is allocated and casted.
   */
  const char *i;   /* Inclusive starting character of token match. */
  const char *j;   /* Exclusive ending   character of token match. */
};

/**
 * @brief Define a namespace for the token structure.
 */
typedef struct token token_t;

/**
 * @brief Define a namespace for the syntax token structure.
 */
typedef token_t syntax_token_t;

void compile(const char *filepath);

#ifdef __cplusplus
}
#endif

#endif/*X_LEXER_H*/
