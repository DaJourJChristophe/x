#include "bind.h"
#include "diagnostic.h"
#include "error.h"
#include "expr.h"
#include "facts.h"

#include <stddef.h>

static const char *get_type_as_string(const binary_expression_t *expr)
{
  if (expr == NULL)
  {
    throw("cannot get string type of expression because it is null");
  }

  const char *boolean_type_str   = "boolean";
  const char *nil_type_str   = "null";
  const char *number_type_str = "number";
  const char *plus_type_str   = " + ";

  switch (expr->type)
  {
    case FALSE:
    case TRUE:     return boolean_type_str;
    case ADDITION: return plus_type_str;
    case NIL:      return nil_type_str;
    case NUMBER:   return number_type_str;
  }

  return "";
}

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
bound_assignment_expression_t *bound_assignment_expression_new(syntax_expression_t *expr)
{
  if (expr->left  == NULL ||
      expr->right == NULL)
  {
    diagnostic_add(diagnostics, DIAGNOSTIC_INVALID_BINARY_EXPRESSION);
  }

  if (( expr->left->kind  != WORD_EXPRESSION) ||
      ( expr->right->kind != BOOLEAN_LITERAL   &&
        expr->right->kind != BINARY_EXPRESSION &&
        expr->right->kind != NUMBER_EXPRESSION))
  {
    char fmt[128];
    memset(fmt, 0, 128 * sizeof(char));
    sprintf(fmt, "cannot perform %s%s%s",
      get_type_as_string(expr->left),
      get_type_as_string(expr),
      get_type_as_string(expr->right));
    diagnostic_add(diagnostics, fmt);
  }

  switch (expr->right->type)
  {
    case TRUE:
      expr->ret_type = TRUE;
      break;

    case FALSE:
      expr->ret_type = FALSE;
      break;

    case NUMBER:
      expr->ret_type = NUMBER;
      break;
  }

  switch (expr->right->type)
  {
    case TRUE:
    case FALSE:
      expr->ret_kind = BOOLEAN_RETURN_TYPE;
      break;

    case NUMBER:
      expr->ret_kind = NUMBER_RETURN_TYPE;
      break;
  }

  return expr;
}

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
bound_binary_expression_t *bound_binary_expression_new(syntax_expression_t *expr)
{
  if (expr->left  == NULL ||
      expr->right == NULL)
  {
    diagnostic_add(diagnostics, DIAGNOSTIC_INVALID_BINARY_EXPRESSION);
  }

  if (( expr->left->kind  != BOOLEAN_LITERAL   &&
        expr->left->kind  != NUMBER_EXPRESSION &&
        expr->left->kind  != WORD_EXPRESSION) ||
      ( expr->right->kind != BOOLEAN_LITERAL   &&
        expr->right->kind != NUMBER_EXPRESSION &&
        expr->right->kind  != WORD_EXPRESSION))
  {
    char fmt[128];
    memset(fmt, 0, 128 * sizeof(char));
    sprintf(fmt, "cannot perform %s%s%s",
      get_type_as_string(expr->left),
      get_type_as_string(expr),
      get_type_as_string(expr->right));
    diagnostic_add(diagnostics, fmt);
  }

  if (( expr->left->kind  == BOOLEAN_LITERAL   ||
        expr->left->kind  == NUMBER_EXPRESSION ||
        expr->left->kind  == WORD_EXPRESSION) &&
      ( expr->right->kind == BOOLEAN_LITERAL   ||
        expr->right->kind == NUMBER_EXPRESSION ||
        expr->right->kind == WORD_EXPRESSION))
  {
    expr->ret_type = NUMBER;
  }

  if (( expr->left->kind  == BOOLEAN_LITERAL   ||
        expr->left->kind  == NUMBER_EXPRESSION ||
        expr->left->kind  == WORD_EXPRESSION) &&
      ( expr->right->kind == BOOLEAN_LITERAL   ||
        expr->right->kind == NUMBER_EXPRESSION ||
        expr->right->kind == WORD_EXPRESSION))
  {
    expr->ret_kind = NUMBER_RETURN_TYPE;
  }

  return expr;
}

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
bound_unary_expression_t *bound_unary_expression_new(syntax_expression_t *expr)
{
  if (expr->left  == NULL ||
      expr->right != NULL)
  {
    diagnostic_add(diagnostics, DIAGNOSTIC_INVALID_UNARY_EXPRESSION);
  }
  if (expr->left->type != TRUE  &&
      expr->left->type != FALSE &&
      expr->left->type != NUMBER)
  {
    char fmt[128];
    memset(fmt, 0, 128 * sizeof(char));
    sprintf(fmt, "cannot perform %s%s",
      get_type_as_string(expr),
      get_type_as_string(expr->left));
    diagnostic_add(diagnostics, fmt);
  }
  if (expr->left->type == TRUE  ||
      expr->left->type == FALSE ||
      expr->left->type == NUMBER)
  {
    expr->ret_type = NUMBER;
  }
  if (expr->left->type == TRUE  ||
      expr->left->type == FALSE ||
      expr->left->type == NUMBER)
  {
    expr->ret_kind = NUMBER_RETURN_TYPE;
  }
  return expr;
}
