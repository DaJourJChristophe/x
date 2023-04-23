#ifndef X_BIND_H
#define X_BIND_H

#include "expr.h"

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef binary_expression_t bound_unary_expression_t;

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef binary_expression_t bound_binary_expression_t;

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef binary_expression_t bound_assignment_expression_t;

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
bound_binary_expression_t *bound_binary_expression_new(syntax_expression_t *expr);

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
bound_unary_expression_t *bound_unary_expression_new(syntax_expression_t *expr);

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
bound_assignment_expression_t *bound_assignment_expression_new(syntax_expression_t *expr);

#endif/*X_BIND_H*/
