/**
 * @file expr.c
 * @author Da'Jour J. Christophe (dajour.christophe@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-04-23
 *
 * @copyright Copyright (c) 2023 Da'Jour J. Christophe. All rights reserved.
 */
#include "common.h"
#include "expr.h"
#include "token.h"

#include <string.h>

/**
 * @brief Allocate a new syntax expression and set the value, left child, and right child.
 */
syntax_expression_t *expression_new(int kind, syntax_token_t *value, syntax_expression_t *left, syntax_expression_t *right)
{
  syntax_expression_t *expr = NULL;
  expr = __calloc(1, sizeof(syntax_expression_t));

  expr->kind = kind;
  expr->type = (-1);
  expr->ret_type = (-1);
  expr->ret_kind = (-1);

  expr->data = NULL;
  expr->size = 0;

  if (value != NULL)
  {
    expr->type = value->type;
    expr->size = value->size;

    if (value->data != NULL)
    {
      expr->data = __malloc(expr->size);
      memcpy(expr->data, value->data, expr->size);
    }
  }

  expr->left  = left;
  expr->right = right;

  return expr;
}

/**
 * @brief Deallocate the expression value and the expression data structure.
 */
void expression_destroy(syntax_expression_t *expr)
{
  if (expr != NULL)
  {
    __free(expr->data);
  }

  __free(expr);
}

syntax_expression_t *expression_copy(syntax_expression_t *old_expr)
{
  syntax_expression_t *new_expr = NULL;
  new_expr = (syntax_expression_t *)__malloc(sizeof(syntax_expression_t));

  new_expr->kind = old_expr->kind;
  new_expr->type = old_expr->type;
  new_expr->size = 0;
  new_expr->data = NULL;
  new_expr->left = NULL;
  new_expr->right = NULL;

  if (old_expr->data != NULL)
  {
    new_expr->size = old_expr->size;
    new_expr->data = __malloc(new_expr->size);
    memcpy(new_expr->data, old_expr->data, new_expr->size);
  }

  if (old_expr->left != NULL)
  {
    new_expr->left = expression_copy(old_expr->left);
  }
  if (old_expr->right != NULL)
  {
    new_expr->right = expression_copy(old_expr->right);
  }

  return new_expr;
}

/**
 * @brief Allocate a new syntax expression, set the value, and set both the left and right child pointers to null.
 */
nil_literal_t *nil_literal_new(syntax_token_t *value)
{
  return expression_new(NIL_LITERAL, value, NULL, NULL);
}

/**
 * @brief Allocate a new syntax expression, set the value, and set both the left and right child pointers to null.
 */
boolean_literal_t *boolean_literal_new(syntax_token_t *value)
{
  return expression_new(BOOLEAN_LITERAL, value, NULL, NULL);
}

/**
 * @brief Allocate a new syntax expression, set the value, and set both the left and right child pointers to null.
 */
number_expression_t *number_expression_new(syntax_token_t *value)
{
  return expression_new(NUMBER_LITERAL, value, NULL, NULL);
}

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
binary_expression_t *binary_expression_new(syntax_token_t *operator, syntax_expression_t *left, syntax_expression_t *right)
{
  return expression_new(BINARY_EXPRESSION, operator, left, right);
}

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
unary_expression_t *unary_expression_new(syntax_token_t *operator)
{
  return expression_new(UNARY_EXPRESSION, operator, NULL, NULL);
}

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
word_expression_t *word_expression_new(syntax_token_t *word)
{
  return expression_new(WORD_LITERAL, word, NULL, NULL);
}

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
assignment_expression_t *assignment_expression_new(syntax_token_t *operator, syntax_expression_t *left, syntax_expression_t *right)
{
  return expression_new(ASSIGNMENT_EXPRESSION, operator, left, right);
}

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
declaration_expression_t *declaration_expression_new(syntax_token_t *type)
{
  return expression_new(DECLARATION_LITERAL, type, NULL, NULL);
}

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
variable_expression_t *variable_expression_new(syntax_token_t *name)
{
  return expression_new(VARIABLE_LITERAL, name, NULL, NULL);
}

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
integer_expression_t *integer_expression_new(syntax_token_t *value)
{
  return expression_new(INTEGER_LITERAL, value, NULL, NULL);
}
