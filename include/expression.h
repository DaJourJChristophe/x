#ifndef X_SYNTAX_EXPRESSION_H
#define X_SYNTAX_EXPRESSION_H

#include "token.h"

/**
 * @brief A collection of syntax expression datatypes.
 */
enum
{
  BINARY_EXPRESSION,
  NUMBER_EXPRESSION,
};

struct syntax_node
{
  int kind;
  syntax_token_t *value;       /* A data structure containing the type and data for the syntax token. */

  struct syntax_node *left;    /* A left  child pointer to a syntax node. */
  struct syntax_node *right;   /* A right child pointer to a syntax node. */
};

/**
 * @brief Define a namespace for the syntax node structure.
 */
typedef struct syntax_node syntax_node_t;

/**
 * @brief Define a namespace for the syntax expression structure.
 */
typedef syntax_node_t syntax_expression_t;

#endif/*X_SYNTAX_EXPRESSION_H*/
