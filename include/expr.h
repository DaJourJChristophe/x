#ifndef X_SYNTAX_EXPRESSION_H
#define X_SYNTAX_EXPRESSION_H

#include "common.h"
#include "token.h"

#include <stddef.h>

/**
 * @brief A collection of syntax expression datatypes.
 */
enum
{
  /**
   * @brief A collection of syntax expression datatypes.
   */
  ASSIGNMENT_EXPRESSION,
  BINARY_EXPRESSION,
  UNARY_EXPRESSION,

  /**
   * @brief A collection of syntax expression datatypes.
   */
  BOOLEAN_LITERAL,
  DECLARATION_LITERAL,
  INTEGER_LITERAL,
  NIL_LITERAL,
  NUMBER_LITERAL,
  STRING_LITERAL,
  VARIABLE_LITERAL,
  WORD_LITERAL,
};

/**
 * @brief A collection of syntax expression datatypes.
 */
struct syntax_node
{
  int kind;                    /* A definition to describe the type of expression. */
  int type;                    /* A definition to describe the data store in the expression. */
  int ret_type;                /* A definition to describe the resulting datatype of the executed expression. */
  int ret_kind;                /* A definition to describe the resulting expression the previously executed expression. */

  void *data;                  /* A pointer to the expression data. */
  size_t size;                 /* The size of the expression data. */

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
 * @brief Define a namespace for the assignment expression structure.
 */
typedef syntax_expression_t assignment_expression_t;

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef syntax_expression_t binary_expression_t;

/**
 * @brief Define a namespace for the boolean expression structure.
 */
typedef syntax_expression_t boolean_literal_t;

/**
 * @brief Define a namespace for the declaration expression structure.
 */
typedef syntax_expression_t declaration_expression_t;

/**
 * @brief Define a namespace for the integer expression structure.
 */
typedef syntax_expression_t integer_expression_t;

/**
 * @brief Define a namespace for the nil expression structure.
 */
typedef syntax_expression_t nil_literal_t;

/**
 * @brief Define a namespace for the number expression structure.
 */
typedef syntax_expression_t number_expression_t;

/**
 * @brief Define a namespace for the unary expression structure.
 */
typedef syntax_expression_t unary_expression_t;

/**
 * @brief Define a namespace for the variable expression structure.
 */
typedef syntax_expression_t variable_expression_t;

/**
 * @brief Define a namespace for the word expression structure.
 */
typedef syntax_expression_t word_expression_t;

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
assignment_expression_t *assignment_expression_new(syntax_token_t *operator, syntax_expression_t *left, syntax_expression_t *right);

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
binary_expression_t *binary_expression_new(syntax_token_t *operator, syntax_expression_t *left, syntax_expression_t *right);

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
boolean_literal_t *boolean_literal_new(syntax_token_t *value);

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
declaration_expression_t *declaration_expression_new(syntax_token_t *type);

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
integer_expression_t *integer_expression_new(syntax_token_t *value);

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
nil_literal_t *nil_literal_new(syntax_token_t *value);

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
number_expression_t *number_expression_new(syntax_token_t *value);

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
unary_expression_t *unary_expression_new(syntax_token_t *operator);

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
variable_expression_t *variable_expression_new(syntax_token_t *name);

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
word_expression_t *word_expression_new(syntax_token_t *word);

/**
 * @brief Allocate a new syntax expression and set the value, left child, and right child.
 */
syntax_expression_t *expression_new(int kind, syntax_token_t *value, syntax_expression_t *left, syntax_expression_t *right);

/**
 * @brief Deallocate the expression value and the expression data structure.
 */
void expression_destroy(syntax_expression_t *expr);

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
syntax_expression_t *expression_copy(syntax_expression_t *old_expr);

#endif/*X_SYNTAX_EXPRESSION_H*/
