#include "common.h"
#include "expr.h"
#include "token.h"

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
boolean_literal_t *boolean_literal_new(syntax_token_t *value)
{
  return expression_new(BOOLEAN_LITERAL, value, NULL, NULL);
}

/**
 * @brief Allocate a new syntax expression, set the value, and set both the left and right child pointers to null.
 */
number_expression_t *number_expression_new(syntax_token_t *value)
{
  return expression_new(NUMBER_EXPRESSION, value, NULL, NULL);
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
