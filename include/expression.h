#ifndef X_SYNTAX_EXPRESSION_H
#define X_SYNTAX_EXPRESSION_H

#include "common.h"
#include "token.h"

#include <string.h>

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

/**
 * @brief Allocate a new syntax expression and set the value, left child, and right child.
 */
static inline syntax_expression_t * always_inline expression_new(int kind, syntax_token_t *value, syntax_expression_t *left, syntax_expression_t *right)
{
  syntax_expression_t *expression = NULL;
  expression = __calloc(1, sizeof(syntax_expression_t));

  expression->kind = kind;

  expression->value = __calloc(1, sizeof(syntax_token_t));

  if (value != NULL)
  {
    memcpy(expression->value, value, sizeof(syntax_token_t));
  }

  // expression->value = value;
  expression->left = left;
  expression->right = right;

  return expression;
}

/**
 * @brief Deallocate the expression value and the expression data structure.
 */
static inline void always_inline expression_destroy(syntax_expression_t *expression)
{
  __free(expression->value);
  __free(expression);
}

#endif/*X_SYNTAX_EXPRESSION_H*/
