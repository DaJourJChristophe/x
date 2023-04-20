#ifndef X_UTILS_H
#define X_UTILS_H

#include "expr.h"
#include "token.h"

/**
 * @brief Print a Syntax Token for development purposes.
 */
void print_token(syntax_token_t *token);

/**
 * @brief Print an Expression for development purposes.
 */
void print_expression(syntax_expression_t *expr);

/**
 * @brief Print a short-hand Expression for development purposes.
 */
void print_expr(syntax_expression_t *expr);

#endif/*X_UTILS_H*/
