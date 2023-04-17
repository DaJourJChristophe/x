#include "common.h"
#include "expr.h"

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
 * @brief Allocate a new syntax expression and set the value, left child, and right child.
 */
syntax_expression_t *expression_new(int kind, syntax_token_t *value, syntax_expression_t *left, syntax_expression_t *right)
{
  syntax_expression_t *expr = NULL;
  expr = __calloc(1, sizeof(syntax_expression_t));

  expr->kind = kind;
  expr->type = (-1);
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
