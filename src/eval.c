#include "expr.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>

/**
 * @brief Print a short-hand Expression for development purposes.
 */
void print_expr(syntax_expression_t *expr);

/**
 * @brief Define a collection of character tokens.
 */
enum
{
  /**
   * @brief General Purpose Tokens.
   */
  DOT,
  EOE,
  LEXER_EOF,
  EOL,
  SPACE,
  STAR,
  SYMBOL,
  TAB,
  TEXT,
  UNDEFINED,
  WORD,

  /**
   * @brief Numeric Tokens.
   */
  DECIMAL,
  NUMBER,

  /**
   * @brief Mathematical Operator Tokens.
   */
  ADDITION,
  DIVISION,
  EQUAL,
  EXPONENTIAL,
  MODULUS,
  REMAINDER,
  SUBTRACTION,

  /**
   * @brief Bitwise Operator Tokens.
   */
  BITWISE_AND,
  BITWISE_OR,
  BITWISE_TERNARY,
  BITWISE_XOR,
  BITWISE_SHIFT_LEFT,
  BITWISE_SHIFT_RIGHT,

  /**
   * @brief Conditional Tokens.
   */
  CONDITIONAL_AND,
  CONDITIONAL_OR,

  /**
   * @brief Containment Tokens.
   */
  LEFT_CARET,
  OPEN_CURLY_BRACKET,
  OPEN_PARENTHESIS,
  OPEN_SQUARE_BRACKET,
  CLOSE_CURLY_BRACKET,
  CLOSE_PARENTHESIS,
  CLOSE_SQUARE_BRACKET,
  RIGHT_CARET,

  /**
   * @brief Architecture Tokens.
   */
  LAMBDA,

  /**
   * @brief Comparator Tokens.
   */
  EQUALS,

  /**
   * @brief Iterator Tokens.
   */
  DECREMENT,
  INCREMENT,

  /**
   * @brief Separator Tokens.
   */
  COLON,
  COMMA,

  /**
   * @brief Special Tokens.
   */
  ANNOTATION,

  /**
   * @brief Reserved Word Tokens.
   */
  ABSTRACT,
  BOOLEAN,
  BREAK,
  CASE,
  CLASS,
  CONST,
  DEFAULT,
  DOUBLE,
  EXPORT,
  FALSE,
  FLOAT,
  FOR,
  IF,
  IMMUTABLE,
  IMPORT,
  INTEGER,
  IS,
  MATRIX,
  NIL,
  OBJECT,
  PACKAGE,
  PRINT,
  PRIVATE,
  PROTECTED,
  PUBLIC,
  RETURN,
  SCALAR,
  SET,
  STATIC,
  STRING,
  SWITCH,
  TRUE,
  UNLESS,
  VECTOR,
  VOID,
  WHILE,
  YIELD,
};

static int eval_calc(syntax_expression_t *root)
{
  if (root == NULL)
  {
    return 0;
  }

  if (root->left  == NULL &&
      root->right == NULL)
  {
    return *(int *)root->data;
  }

  int leftEval  = eval_calc(root->left);
  int rightEval = eval_calc(root->right);

  switch (root->kind)
  {
    case UNARY_EXPRESSION:        return -leftEval;

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
        case BITWISE_TERNARY:     return ~leftEval;
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
