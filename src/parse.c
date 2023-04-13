#include "common.h"
#include "lexer.h"
#include "token.h"
#include "cache.h"
#include "expression.h"
#include "syntax-queue.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Print a Syntax Token for development purposes.
 */
void print_token(syntax_token_t *token);

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
 * @brief Allocate a new syntax expression and set the value, left child, and right child.
 */
syntax_expression_t *expression_new(int kind, syntax_token_t *value, syntax_expression_t *left, syntax_expression_t *right)
{
  syntax_expression_t *expression = NULL;
  expression = __calloc(1, sizeof(syntax_expression_t));

  expression->kind = kind;

  expression->value = __calloc(1, sizeof(syntax_token_t));

  memcpy(expression->value, value, sizeof(syntax_token_t));

  // expression->value = value;
  expression->left = left;
  expression->right = right;

  return expression;
}

/**
 * @brief Deallocate the expression value and the expression data structure.
 */
void expression_destroy(syntax_expression_t *expression)
{
  __free(expression->value);
  __free(expression);
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

void parse(syntax_queue_t *queue)
{
  syntax_token_t *token = NULL;

  number_expression_t *expression1 = NULL;
  number_expression_t *expression2 = NULL;
  number_expression_t *expression3 = NULL;

  cache_t *cache = cache_new();

  syntax_token_t a;
  syntax_token_t b;

  syntax_token_t *temp;


  do
  {
    token = syntax_queue_read(queue);

    if (token != NULL)
    {
      switch (token->type)
      {
        case LEXER_EOF:
          return;

        case EOL:
        case TAB:
        case SPACE:
          break;

        case ADDITION:
          if (cache_peek(cache) != NULL)
          {
            if (cache_peek(cache)->type == NUMBER)
            {
              cache_add(cache, cache_node_new(token));
            }
          }
          break;

        case NUMBER:
          if (cache_peek(cache) == NULL)
          {
            cache_add(cache, cache_node_new(token));
            break;
          }
          else
          {
            if (cache_peek(cache)->type != ADDITION)
            {
              cache_add(cache, cache_node_new(token));
              break;
            }
            else
            {
              temp = cache_peek(cache);
              memcpy(&a, temp, sizeof(syntax_token_t));
              cache_pop(cache);
              temp = NULL;

              temp = cache_peek(cache);
              memcpy(&b, temp, sizeof(syntax_token_t));
              cache_pop(cache);
              temp = NULL;

              expression1 = number_expression_new(&b);
              expression2 = number_expression_new(token);

              expression3 = binary_expression_new(&a, expression1, expression2);

              if (expression3->kind == BINARY_EXPRESSION)
              {
                switch ( expression3->value->type )
                {
                  case ADDITION:
                    printf("%d\n",
                      *(int *)expression3->left->value->data +
                      *(int *)expression3->right->value->data);
                    break;
                }
              }

              expression_destroy(expression1);
              expression_destroy(expression2);
              expression_destroy(expression3);
            }
          }
          break;

        default:
          die("unknown token kind", __FILE__, __func__);
      }
    }
  }
  while (token != NULL);
}
