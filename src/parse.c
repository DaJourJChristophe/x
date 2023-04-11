#include "common.h"
#include "lexer.h"
#include "token.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Print a Syntax Token for development purposes.
 */
void print_token(syntax_token_t *token);

/**
 * @brief A collection of syntax expression datatypes.
 */
enum
{
  BinaryExpression,
  NumberExpression,
};

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

struct syntax_node
{
  syntax_token_t *value;       /* A data structure containing the type and data for the syntax token. */

  struct syntax_node *left;    /* A left  child pointer to a syntax node. */
  struct syntax_node *right;   /* A right child pointer to a syntax node. */
};

/**
 * @brief Define a namespace for the syntax node structure.
 */
typedef struct syntax_node syntax_node_t;

/**
 * @brief Define a namespace for the syntax expression structure.
 */
typedef syntax_node_t syntax_expression_t;

/**
 * @brief Allocate a new syntax expression and set the value, left child, and right child.
 */
syntax_expression_t *expression_new(syntax_token_t *value, syntax_expression_t *left, syntax_expression_t *right)
{
  syntax_expression_t *expression = NULL;
  expression = __calloc(1, sizeof(syntax_expression_t));
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
  return expression_new(value, NULL, NULL);
}

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef syntax_expression_t binary_expression_t;

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
binary_expression_t *binary_expression_new(syntax_token_t *operator, syntax_token_t *left, syntax_token_t *right)
{
  return expression_new(operator, number_expression_new(left), number_expression_new(right));
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

  number_expression_t *expression = NULL;

  syntax_token_t *prev_prev;
  syntax_token_t *prev;

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
          puts("END OF LINE TOKEN");
          break;

        case NUMBER:
          // expression = number_expression_new(token);
          // expression_destroy(expression);

          prev_prev = token; /* NOTE: Cache token in a splay tree. */

          do
          {
            token = syntax_queue_read(queue);
          }
          while ( token != NULL && token->type == SPACE );

          if (token != NULL)
          {
            if (token->type == ADDITION)
            {
              prev = token;

              do
              {
                token = syntax_queue_read(queue);
              }
              while ( token != NULL && token->type == SPACE );

              if (token != NULL)
              {
                if (token->type == NUMBER)
                {
                  puts("BINARY EXPRESSION");
                  expression = binary_expression_new(prev, prev_prev, token);
                  expression_destroy(expression);
                }
              }
            }
          }
          break;

        default:
          die("unknown token kind", __FILE__, __func__);
      }
    }
  }
  while (token != NULL);

  // binary_expression_t *expression = binary_expression_new(token_o, token_a, token_b);

  // printf("%d\n", expression->value->type);
  // printf("%d\n", *(int *)expression->value->data);

  // expression_destroy(expression);
}
