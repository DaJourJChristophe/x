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
  UNARY_EXPRESSION,
  WORD_EXPRESSION,
  ASSIGNMENT_EXPRESSION,
  DECLARATION_EXPRESSION,
  VARIABLE_EXPRESSION,
  INTEGER_EXPRESSION,

  NIL_LITERAL,
  BOOLEAN_LITERAL,
};

struct syntax_node
{
  int kind;
  int type;
  int ret_type;
  int ret_kind;

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

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef syntax_expression_t nil_literal_t;

nil_literal_t *nil_literal_new(syntax_token_t *value);

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef syntax_expression_t boolean_literal_t;

boolean_literal_t *boolean_literal_new(syntax_token_t *value);

/**
 * @brief Define a namespace for the number expression structure.
 */
typedef syntax_expression_t number_expression_t;

number_expression_t *number_expression_new(syntax_token_t *value);

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef syntax_expression_t binary_expression_t;

binary_expression_t *binary_expression_new(syntax_token_t *operator, syntax_expression_t *left, syntax_expression_t *right);

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef syntax_expression_t unary_expression_t;

unary_expression_t *unary_expression_new(syntax_token_t *operator);

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef syntax_expression_t word_expression_t;

word_expression_t *word_expression_new(syntax_token_t *word);

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef syntax_expression_t assignment_expression_t;

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
assignment_expression_t *assignment_expression_new(syntax_token_t *operator, syntax_expression_t *left, syntax_expression_t *right);

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef syntax_expression_t declaration_expression_t;

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
declaration_expression_t *declaration_expression_new(syntax_token_t *type);

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef syntax_expression_t variable_expression_t;

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
variable_expression_t *variable_expression_new(syntax_token_t *name);

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef syntax_expression_t integer_expression_t;

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
integer_expression_t *integer_expression_new(syntax_token_t *value);

#endif/*X_SYNTAX_EXPRESSION_H*/
