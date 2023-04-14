#include "common.h"
#include "lexer.h"
#include "token.h"
#include "cache.h"
#include "expression.h"
#include "syntax-queue.h"
#include "syntax-stack.h"
#include "ast.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Allocate a new syntax token and set both the type and the data;
 */
syntax_token_t *syntax_token_new(int type, void *data, size_t const n)
{
  syntax_token_t *token = NULL;
  token = __calloc(1, sizeof(syntax_token_t));

  token->type = type;
  token->data = __malloc(n);

  memcpy(token->data, data, n);

  return token;
}

/**
 * @brief Destroy the token and the token data.
 */
void syntax_token_destroy(syntax_token_t *token)
{
  __free(token->data);
  __free(token);
}

/**
 * @brief Define a namespace for the number expression structure.
 */
typedef syntax_expression_t number_expression_t;

/**
 * @brief Allocate a new syntax expression, set the value, and set both the left and right child pointers to null.
 */
number_expression_t *number_expression_new(syntax_token_t *value)
{
  return expression_new(NUMBER_EXPRESSION, value, NULL, NULL);
}

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef syntax_expression_t binary_expression_t;

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
binary_expression_t *binary_expression_new(syntax_token_t *operator, syntax_expression_t *left, syntax_expression_t *right)
{
  return expression_new(BINARY_EXPRESSION, operator, left, right);
}

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

int precedence(int kind)
{
  switch (kind)
  {
    case STAR:
      return 3;

    case DIVISION:
      return 2;

    case ADDITION:
      return 1;

    case SUBTRACTION:
      return 0;
  }

  return (-1);
}

void parse(syntax_queue_t *queue)
{
  syntax_token_t *token = NULL;
  syntax_token_t *temp;

  syntax_stack_t *symbol_stack = syntax_stack_new(64);
  // syntax_queue_t *number_queue = syntax_queue_new(64);

  abstract_syntax_tree_t *tree = NULL;
  tree = abstract_syntax_tree_new();

  // binary_expression_t *bexpr = NULL;

  do
  {
    token = syntax_queue_read(queue);

    if (token != NULL)
    {
      switch (token->type)
      {
        case LEXER_EOF:
          break;

        case EOE:
          while ((temp = syntax_stack_pop(symbol_stack)) != NULL)
          {
            abstract_syntax_tree_insert(tree, BINARY_EXPRESSION, temp);
            // if (queue_write(number_queue, temp) == false)
            // {
            //   die("failed to write to syntax queue", __FILE__, __func__);
            // }
          }
          // bexpr = binary_expression_new(NULL, NULL, NULL);
          // while ((temp = syntax_queue_read(number_queue)) != NULL)
          // {
          //   switch (temp->type)
          //   {
          //     case NUMBER:
          //       if (bexpr->left == NULL)
          //       {
          //         bexpr->left = number_expression_new(temp);
          //       }
          //       else
          //       {
          //         bexpr->right = number_expression_new(temp);
          //       }
          //       break;

          //     case ADDITION:
          //       memcpy(bexpr->value, temp, sizeof(syntax_token_t));
          //       // bexpr->value = temp;
          //       break;
          //   }
          // }
          break;

        case EOL:
        case TAB:
        case SPACE:
          break;

        case ADDITION:
          if (syntax_stack_push(symbol_stack, token) == false)
          {
            die("failed to push to syntax stack", __FILE__, __func__);
          }
          break;

        case NUMBER:
          abstract_syntax_tree_insert(tree, NUMBER_EXPRESSION, token);
          // if (syntax_queue_write(number_queue, token) == false)
          // {
          //   die("failed to write to syntax queue", __FILE__, __func__);
          // }
          break;

        default:
          die("unknown token kind", __FILE__, __func__);
      }
    }
  }
  while (token != NULL);

  printLevelOrder(tree->root);

  // expression_destroy(bexpr);

  abstract_syntax_tree_destroy(tree);

  syntax_stack_destroy(symbol_stack);
  // syntax_queue_destroy(number_queue);
}
