#include "common.h"
#include "lexer.h"
#include "token.h"
#include "cache.h"
#include "expr.h"
#include "syntax-expression-queue.h"
#include "syntax-queue.h"
#include "syntax-expression-stack.h"
#include "ast.h"
#include "error.h"

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

/*
manually set node left upon new_node
only set node right
*/
static syntax_expression_t *__insert(syntax_expression_t *node,
                                     syntax_expression_t *new_node)
{
  if (node == NULL)
  {
    return new_node;
  }
  if (node->left == NULL)
  {
    node->left = __insert(node->left, new_node);
    return node;
  }
  node->right = __insert(node->right, new_node);
  return node;
}

static int max(int a, int b)
{
  return a > b ? a : b;
}

static int height(syntax_expression_t *node)
{
  if (node == NULL)
  {
    return 0;
  }
  return max(height(node->left), height(node->right)) + 1;
}

static int getcol(int h)
{
  if (h == 1)
  {
    return 1;
  }
  return getcol(h - 1) + getcol(h - 1) + 1;
}

static void _print_tree(syntax_expression_t ***M, syntax_expression_t *root, int col, int row, int height)
{
  if (root == NULL)
  {
    return;
  }

  M[row][col] = expression_copy(root);

  _print_tree(M, root->left,  col - pow(2, height - 2), row + 1, height - 1);
  _print_tree(M, root->right, col + pow(2, height - 2), row + 1, height - 1);
}

void print_tree(syntax_expression_t *root)
{
  int h = height(root);
  int col = getcol(h);
  syntax_expression_t ***M;

  M = __calloc(h, sizeof(syntax_expression_t **));

  for (int i = 0; i < h; i++)
  {
    M[i] = __calloc(col, sizeof(syntax_expression_t *));
  }

  _print_tree(M, root, col / 2, 0, h);

  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < col; j++)
    {
      if (M[i][j] == NULL)
      {
        printf(" ");
      }
      else
      {
        print_expr(M[i][j]);
      }
    }
    printf("\n");
  }
}

// static void ___traverse(syntax_expression_t *node, char *indent, int index)
// {
//   if (node == NULL)
//   {
//     return;
//   }

//   printf(indent);

//   print_expression(node);

// // └──
// // │
// // ├──

//   ___traverse(node->left, indent, index);

//   indent[index++] = ' ';
//   indent[index++] = ' ';

//   ___traverse(node->right, indent, index);
// }

// static void __traverse(syntax_expression_t *node)
// {
//   char indent[64];
//   memset(indent, 0, (64 * sizeof(char)));
//   ___traverse(node, indent, 0);
// }

/*
3 + 4 + 5

    +
   / \
  3   +
     / \
    4   5
*/

void parse(syntax_queue_t *queue)
{
  syntax_token_t *token = NULL;
  syntax_expression_t *expr = NULL;
  syntax_expression_t *temp = NULL;
  syntax_expression_t *root = NULL;

  syntax_expression_queue_t *number_queue = syntax_expression_queue_new(64);
  syntax_expression_stack_t *symbol_stack = syntax_expression_stack_new(64);

  syntax_expression_stack_t *expr_stack = syntax_expression_stack_new(64);

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
          if (syntax_expression_queue_write(number_queue, temp) == false)
          {
            throw(X_ERROR_SYNTAX_QUEUE_WRITE);
          }
          expression_destroy(temp);
          temp = NULL;
        }
        while ((temp = syntax_expression_queue_read(number_queue)) != NULL)
        {
          switch (temp->kind)
          {
            case BINARY_EXPRESSION:
              temp->left = syntax_expression_stack_pop(symbol_stack);
              temp->right = syntax_expression_stack_pop(symbol_stack);
              if (syntax_expression_stack_push(expr_stack, temp) == false)
              {
                throw("failed to push to the expr stack");
              }
              break;

            case NUMBER_EXPRESSION:
              if (syntax_expression_stack_push(symbol_stack, temp) == false)
              {
                throw("failed to push to the symbol stack");
              }
              break;
          }
        }
        while ((temp = syntax_expression_stack_pop(expr_stack)) != NULL)
        {
          root = __insert(root, temp);
        }
        print_tree(root);
        break;

      case EOL:
      case TAB:
      case SPACE:
        // print_token(token);
        break;

      case ADDITION:
      case STAR:
        expr = binary_expression_new(token, NULL, NULL);
        temp = syntax_expression_stack_peek(symbol_stack);

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
            if (syntax_expression_queue_write(number_queue, temp) == false)
            {
              throw(X_ERROR_SYNTAX_QUEUE_WRITE);
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

        expression_destroy(expr);
        expr = NULL;
        break;

      case NUMBER:
        expr = number_expression_new(token);

        if (syntax_expression_queue_write(number_queue, expr) == false)
        {
          throw(X_ERROR_SYNTAX_QUEUE_WRITE);
        }

        expression_destroy(expr);
        expr = NULL;
        break;

      default:
        throw(X_ERROR_UNSUPPORTED_TOKEN);
    }

    __free(token);
    token = NULL;
  }

  syntax_expression_queue_destroy(number_queue);
  syntax_expression_stack_destroy(symbol_stack);
  syntax_expression_stack_destroy(expr_stack);
}
