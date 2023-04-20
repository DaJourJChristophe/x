#include "expr.h"

#include <stddef.h>

/**
 * @brief Print a short-hand Expression for development purposes.
 */
void print_expr(syntax_expression_t *expr);

void eval(syntax_expression_t *root)
{
  if (root == NULL)
  {
    return;
  }

  eval(root->left);

  print_expr(root);

  eval(root->right);
}
