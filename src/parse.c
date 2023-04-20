#include "common.h"
#include "lexer.h"
#include "token.h"
#include "cache.h"
#include "expr.h"
// #include "syntax-expression-queue.h"
#include "syntax-queue.h"
#include "syntax-expression-stack.h"
#include "ast.h"
#include "error.h"
#include "eval.h"

#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Print a Syntax Token for development purposes.
 */
void print_token(syntax_token_t *token);

/**
 * @brief Print an Expression for development purposes.
 */
void print_expression(syntax_expression_t *expr);

/**
 * @brief Print a short-hand Expression for development purposes.
 */
void print_expr(syntax_expression_t *expr);

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
    case BITWISE_XOR:       return 9;
    case EXPONENTIAL:       return 9;
    case STAR:              return 8;
    case DIVISION:          return 8;
    case MODULUS:           return 8;
    case ADDITION:          return 5;
    case SUBTRACTION:       return 5;
    case OPEN_PARENTHESIS:  return 0;
    case CLOSE_PARENTHESIS: return 0;
  }

  return (-1);
}

syntax_expression_t *parse(syntax_queue_t *queue)
{
  syntax_token_t *token = NULL;
  syntax_expression_t *expr = NULL;
  syntax_expression_t *temp = NULL;
  syntax_expression_t *tamp = NULL;
  syntax_expression_t *root = NULL;

  syntax_expression_stack_t *symbol_stack = syntax_expression_stack_new(64);
  syntax_expression_stack_t *nodes = syntax_expression_stack_new(64);

  int last_expr_kind = (-1);

  while ((token = syntax_queue_read(queue)) != NULL)
  {
    switch (token->type)
    {
      case LEXER_EOF:
        // print_token(token);
        break;

      case EOE:
        while ((temp = syntax_expression_stack_pop(symbol_stack)) != NULL)
        {
          temp->right = syntax_expression_stack_pop(nodes);
          temp->left  = syntax_expression_stack_pop(nodes);

          if (syntax_expression_stack_push(nodes, temp) == false)
          {
            throw("failed to push to the nodes stack");
          }

          expression_destroy(temp);
          temp = NULL;
        }

        root = syntax_expression_stack_pop(nodes);
        break;

      case EOL:
      case TAB:
      case SPACE:
        // print_token(token);
        break;

      case CLOSE_PARENTHESIS:
        while ((temp = syntax_expression_stack_pop(symbol_stack)) != NULL)
        {
          if (temp->type == OPEN_PARENTHESIS)
          {
            expression_destroy(temp);
            temp = NULL;
            break;
          }

          temp->right = syntax_expression_stack_pop(nodes);
          temp->left  = syntax_expression_stack_pop(nodes);

          if (syntax_expression_stack_push(nodes, temp) == false)
          {
            throw("failed to push to the nodes stack");
          }

          expression_destroy(temp);
          temp = NULL;
        }
        break;

      case OPEN_PARENTHESIS:
        expr = binary_expression_new(token, NULL, NULL);

        if (syntax_expression_stack_push(symbol_stack, expr) == false)
        {
          throw("failed to push to the nodes stack");
        }

        last_expr_kind = expr->kind;

        expression_destroy(expr);
        expr = NULL;
        break;

      case ADDITION:
      case DIVISION:
      case EXPONENTIAL:
      case SUBTRACTION:
      case STAR:
        expr = binary_expression_new(token, NULL, NULL);

        temp = syntax_expression_stack_peek(symbol_stack);
        tamp = syntax_expression_stack_peek(nodes);

        if (temp == NULL && tamp == NULL)
        {
          puts("unary operator");
        }
        else if (temp == NULL && tamp != NULL)
        {
          puts("binary operator");
        }
        else if (temp != NULL && tamp != NULL)
        {
          switch (last_expr_kind)
          {
            case BINARY_EXPRESSION:
              puts("unary");
              break;

            default:
              puts("binary operator");
          }
        }

        if (temp == NULL)
        {
          if (syntax_expression_stack_push(symbol_stack, expr) == false)
          {
            throw("failed to push to the symbol stack");
          }
        }
        else
        {
          if (precedence(temp->type) > precedence(expr->type))
          {
            temp->right = syntax_expression_stack_pop(nodes);
            temp->left  = syntax_expression_stack_pop(nodes);

            if (syntax_expression_stack_push(nodes, temp) == false)
            {
              throw("failed to push to the nodes stack");
            }

            expression_destroy(syntax_expression_stack_pop(symbol_stack));
            expression_destroy(temp);
            temp = NULL;
          }

          if (syntax_expression_stack_push(symbol_stack, expr) == false)
          {
            throw("failed to push to the symbol stack");
          }
        }

        last_expr_kind = expr->kind;

        expression_destroy(expr);
        expression_destroy(tamp);
        expr = NULL;
        tamp = NULL;
        break;

      case NUMBER:
        expr = number_expression_new(token);

        if (syntax_expression_stack_push(nodes, expr) == false)
        {
          throw("failed to push to the nodes stack");
        }

        last_expr_kind = expr->kind;

        expression_destroy(expr);
        expr = NULL;
        break;

      default:
        throw(X_ERROR_UNSUPPORTED_TOKEN);
    }

    __free(token);
    token = NULL;
  }

  syntax_expression_stack_destroy(symbol_stack);
  syntax_expression_stack_destroy(nodes);

  return root;
}
