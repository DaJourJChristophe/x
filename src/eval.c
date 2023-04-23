#include "error.h"
#include "expr.h"
#include "facts.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>

/**
 * @brief Print a short-hand Expression for development purposes.
 */
void print_expr(syntax_expression_t *expr);

static int eval_calc(syntax_expression_t *root)
{
  if (root == NULL)
  {
    return 0;
  }

  if (root->left  == NULL &&
      root->right == NULL)
  {
    if (root->data == NULL)
    {
      throw("node data cannot be evaluated because it is null");
    }
    return *(int *)root->data;
  }

  int leftEval  = eval_calc(root->left);
  int rightEval = eval_calc(root->right);

  switch (root->kind)
  {
    case UNARY_EXPRESSION:
      switch (root->type)
      {
        case SUBTRACTION:     return -leftEval;
        case BITWISE_TERNARY: return ~leftEval;
      }
      break;

    case BINARY_EXPRESSION:
      switch (root->type)
      {
        case ADDITION:            return leftEval + rightEval;
        case EXPONENTIAL:         return pow(leftEval, rightEval);
        case SUBTRACTION:         return leftEval - rightEval;
        case STAR:                return leftEval * rightEval;
        case DIVISION:            return leftEval / rightEval;
        case MODULUS:             return leftEval % rightEval;
        case REMAINDER:           return (int)(leftEval / rightEval);

        case BITWISE_AND:         return leftEval & rightEval;
        case BITWISE_OR:          return leftEval | rightEval;
        case BITWISE_XOR:         return leftEval ^ rightEval;
        case BITWISE_SHIFT_LEFT:  return leftEval << rightEval;
        case BITWISE_SHIFT_RIGHT: return leftEval >> rightEval;
      }
  }

  return 0;
}

void eval(syntax_expression_t *root)
{
  printf("%d\n", eval_calc(root));
}
