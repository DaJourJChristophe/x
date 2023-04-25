/**
 * @file bind.c
 * @author Da'Jour J. Christophe (dajour.christophe@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-04-23
 *
 * @copyright Copyright (c) 2023 Da'Jour J. Christophe. All rights reserved.
 */
#include "bind.h"
#include "diagnostic.h"
#include "error.h"
#include "expr.h"
#include "facts.h"

#include <stddef.h>
#include <string.h>

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

  if (( expr->left->kind  != DECLARATION_LITERAL) ||
      ( expr->right->kind != BOOLEAN_LITERAL   &&
        expr->right->kind != BINARY_EXPRESSION &&
        expr->right->kind != NUMBER_LITERAL &&
        expr->right->kind != INTEGER_LITERAL))
  {
    char fmt[128];
    memset(fmt, 0, 128 * sizeof(char));
    sprintf(fmt, "cannot perform %s%s%s",
      get_type_as_string(expr->left),
      get_type_as_string(expr),
      get_type_as_string(expr->right));
    diagnostic_add(diagnostics, fmt);
  }

  if (expr->left->ret_kind != expr->right->ret_kind)
  {
    diagnostic_add(diagnostics, "left-handle declaration of type ... cannot equal a right-hand expression of type ...");
  }
  else
  {
    expr->ret_type = expr->left->type;
    expr->ret_kind = expr->left->ret_kind;
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

  if (( expr->left->kind  != BOOLEAN_LITERAL &&
        expr->left->kind  != INTEGER_LITERAL &&
        expr->left->kind  != NUMBER_LITERAL  &&
        expr->left->kind  != WORD_LITERAL) ||
      ( expr->right->kind != BOOLEAN_LITERAL &&
        expr->right->kind != INTEGER_LITERAL &&
        expr->right->kind != NUMBER_LITERAL  &&
        expr->right->kind != WORD_LITERAL))
  {
    char fmt[128];
    memset(fmt, 0, 128 * sizeof(char));
    sprintf(fmt, "cannot perform %s%s%s",
      get_type_as_string(expr->left),
      get_type_as_string(expr),
      get_type_as_string(expr->right));
    diagnostic_add(diagnostics, fmt);
  }

  if (expr->left->ret_kind  == BOOLEAN_RETURN_TYPE &&
      expr->right->ret_kind == BOOLEAN_RETURN_TYPE)
  {
    expr->ret_type = BOOLEAN;
    expr->ret_kind = BOOLEAN_RETURN_TYPE;
  }
  else if ( expr->left->ret_kind  == INTEGER_RETURN_TYPE &&
            expr->right->ret_kind == INTEGER_RETURN_TYPE)
  {
    expr->ret_type = INTEGER;
    expr->ret_kind = INTEGER_RETURN_TYPE;
  }
  else if ( expr->left->ret_kind  == BOOLEAN_RETURN_TYPE &&
            expr->right->ret_kind == INTEGER_RETURN_TYPE)
  {
    expr->ret_type = INTEGER;
    expr->ret_kind = INTEGER_RETURN_TYPE;
  }
  else if ( expr->left->ret_kind  == INTEGER_RETURN_TYPE &&
            expr->right->ret_kind == BOOLEAN_RETURN_TYPE)
  {
    expr->ret_type = INTEGER;
    expr->ret_kind = INTEGER_RETURN_TYPE;
  }
  else
  {
    throw("could not determine the return type of the binary expression");
  }

  return expr;
}

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
bound_boolean_expression_t *bound_boolean_expression_new(syntax_expression_t *expr)
{
  if (expr->left  != NULL ||
      expr->right != NULL)
  {
    diagnostic_add(diagnostics, DIAGNOSTIC_INVALID_BINARY_EXPRESSION);
  }

  expr->ret_type = expr->type;
  expr->ret_kind = BOOLEAN_RETURN_TYPE;

  return expr;
}

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
bound_declaration_expression_t *bound_declaration_expression_new(syntax_expression_t *expr)
{
  if (expr->left  != NULL ||
      expr->right != NULL)
  {
    diagnostic_add(diagnostics, DIAGNOSTIC_INVALID_BINARY_EXPRESSION);
  }

  expr->ret_type = expr->type;

  switch (expr->ret_type)
  {
    case BOOLEAN:
      expr->ret_kind = BOOLEAN_RETURN_TYPE;
      break;

    case INTEGER:
      expr->ret_kind = INTEGER_RETURN_TYPE;
      break;

    default:
      throw("unrecognized return type");
  }

  return expr;
}

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
bound_integer_expression_t *bound_integer_expression_new(syntax_expression_t *expr)
{
  if (expr->left  != NULL ||
      expr->right != NULL)
  {
    diagnostic_add(diagnostics, DIAGNOSTIC_INVALID_BINARY_EXPRESSION);
  }

  expr->ret_type = expr->type;
  expr->ret_kind = INTEGER_RETURN_TYPE;

  return expr;
}

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
bound_number_expression_t *bound_number_expression_new(syntax_expression_t *expr)
{
  if (expr->left  != NULL ||
      expr->right != NULL)
  {
    diagnostic_add(diagnostics, DIAGNOSTIC_INVALID_BINARY_EXPRESSION);
  }

  expr->ret_type = expr->type;
  expr->ret_kind = NUMBER_RETURN_TYPE;

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

  if (expr->left->ret_kind != BOOLEAN_RETURN_TYPE  &&
      expr->left->ret_kind != INTEGER_RETURN_TYPE)
  {
    char fmt[128];
    memset(fmt, 0, 128 * sizeof(char));
    sprintf(fmt, "cannot perform %s%s",
      get_type_as_string(expr),
      get_type_as_string(expr->left));
    diagnostic_add(diagnostics, fmt);
  }

  switch (expr->type)
  {
    case DECREMENT:
    case INCREMENT:
    case SUBTRACTION:
      expr->ret_type = INTEGER_RETURN_TYPE;
      break;

    default:
      throw("could not determine the return of the unary expression");
  }

  return expr;
}
