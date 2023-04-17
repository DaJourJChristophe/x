#ifndef X_SYNTAX_EXPRESSION_H
#define X_SYNTAX_EXPRESSION_H

#include "common.h"
#include "token.h"

#include <stddef.h>
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
  int type;
  void *data;
  size_t size;

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
syntax_expression_t *expression_new(int kind, syntax_token_t *value, syntax_expression_t *left, syntax_expression_t *right);

/**
 * @brief Deallocate the expression value and the expression data structure.
 */
void expression_destroy(syntax_expression_t *expr);

syntax_expression_t *expression_copy(syntax_expression_t *old_expr);

#endif/*X_SYNTAX_EXPRESSION_H*/
