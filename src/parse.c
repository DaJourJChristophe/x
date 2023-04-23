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
#include "facts.h"
#include "utils.h"

#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef binary_expression_t bound_unary_expression_t;

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef binary_expression_t bound_binary_expression_t;

const char *get_type_as_string(const binary_expression_t *expr)
{
  if (expr == NULL)
  {
    throw("cannot get string type of expression because it is null");
  }

  const char *boolean_type_str   = "boolean";
  const char *number_type_str = "number";
  const char *plus_type_str   = " + ";

  switch (expr->type)
  {
    case FALSE:
    case TRUE:     return boolean_type_str;
    case ADDITION: return plus_type_str;
    case NUMBER:   return number_type_str;
  }

  return NULL;
}

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
bound_binary_expression_t *bound_binary_expression_new(syntax_expression_t *expr)
{
  if (expr->left  == NULL ||
      expr->right == NULL)
  {
    printf("%s\n", "invalid binary expression");
  }
  if (( expr->left->type  != TRUE  &&
        expr->left->type  != FALSE &&
        expr->left->type  != NUMBER) ||
      ( expr->right->type != TRUE  &&
        expr->right->type != FALSE &&
        expr->right->type != NUMBER))
  {
    char fmt[128];
    memset(fmt, 0, 128 * sizeof(char));
    sprintf(fmt, "cannot perform %s%s%s\n",
      get_type_as_string(expr->left),
      get_type_as_string(expr),
      get_type_as_string(expr->right));
    fprintf(stdout, fmt, "");
  }
  if (( expr->left->type  == TRUE  ||
        expr->left->type  == FALSE ||
        expr->left->type  == NUMBER) &&
      ( expr->right->type == TRUE  ||
        expr->right->type == FALSE ||
        expr->right->type == NUMBER))
  {
    expr->ret_type = NUMBER;
  }
  return expr;
}

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
bound_binary_expression_t *bound_unary_expression_new(syntax_expression_t *expr)
{
  if (expr->left  == NULL ||
      expr->right != NULL)
  {
    printf("%s\n", "invalid unary expression");
  }
  if (expr->left->type != TRUE  &&
      expr->left->type != FALSE &&
      expr->left->type != NUMBER)
  {
    fprintf(stdout, "cannot perform %s%s%s\n",
      get_type_as_string(expr->left),
      get_type_as_string(expr),
      get_type_as_string(expr->right));
  }
  if (expr->left->type == TRUE  ||
      expr->left->type == FALSE ||
      expr->left->type == NUMBER)
  {
    expr->ret_type = NUMBER;
  }
  return expr;
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
          switch (temp->kind)
          {
            case UNARY_EXPRESSION:
              temp->left  = syntax_expression_stack_pop(nodes);

              temp = bound_unary_expression_new(temp);

              if (syntax_expression_stack_push(nodes, temp) == false)
              {
                throw("failed to push to the nodes stack");
              }
              break;

            case BINARY_EXPRESSION:
              temp->right = syntax_expression_stack_pop(nodes);
              temp->left  = syntax_expression_stack_pop(nodes);

              temp = bound_binary_expression_new(temp);

              if (syntax_expression_stack_push(nodes, temp) == false)
              {
                throw("failed to push to the nodes stack");
              }
              break;
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

          temp = bound_binary_expression_new(temp);

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

      case TRUE:
      case FALSE:
        expr = boolean_literal_new(token);

        if (syntax_expression_stack_push(nodes, expr) == false)
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
      case MODULUS:
      case REMAINDER:
      case SUBTRACTION:
      case STAR:

      case BITWISE_AND:
      case BITWISE_OR:
      case BITWISE_TERNARY:
      case BITWISE_XOR:
      case BITWISE_SHIFT_LEFT:
      case BITWISE_SHIFT_RIGHT:

        temp = syntax_expression_stack_peek(symbol_stack);
        tamp = syntax_expression_stack_peek(nodes);

        if ((temp == NULL && tamp == NULL) ||
            (temp != NULL && tamp == NULL))
        {
          expr = unary_expression_new(token);

          if (syntax_expression_stack_push(symbol_stack, expr) == false)
          {
            throw("failed to push to the symbol stack");
          }
        }
        else if ( temp == NULL &&
                  tamp != NULL)
        {
          expr = binary_expression_new(token, NULL, NULL);

          if (syntax_expression_stack_push(symbol_stack, expr) == false)
          {
            throw("failed to push to the symbol stack");
          }

          expression_destroy(tamp);
          tamp = NULL;
        }
        else if ( temp != NULL &&
                  tamp != NULL)
        {
          switch (last_expr_kind)
          {
            case BINARY_EXPRESSION:
              expr = unary_expression_new(token);

              if (syntax_expression_stack_push(symbol_stack, expr) == false)
              {
                throw("failed to push to the symbol stack");
              }
              break;

            default:
              expr = binary_expression_new(token, NULL, NULL);

              if (precedence(temp->type) > precedence(expr->type))
              {
                temp->right = syntax_expression_stack_pop(nodes);
                temp->left  = syntax_expression_stack_pop(nodes);

                temp = bound_binary_expression_new(temp);

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

          expression_destroy(tamp);
          tamp = NULL;
        }

        last_expr_kind = expr->kind;
        expression_destroy(expr);
        expr = NULL;
        break;

      case NUMBER:
        expr = number_expression_new(token);
        temp = syntax_expression_stack_peek(symbol_stack);

        if (temp != NULL)
        {
          switch (temp->kind)
          {
            case UNARY_EXPRESSION:
              temp->left = expr;

              temp = bound_unary_expression_new(temp);

              if (syntax_expression_stack_push(nodes, temp) == false)
              {
                throw("failed to push to the nodes stack");
              }

              expression_destroy(syntax_expression_stack_pop(symbol_stack));
              break;

            default:
              if (syntax_expression_stack_push(nodes, expr) == false)
              {
                throw("failed to push to the nodes stack");
              }
          }

          expression_destroy(temp);
          temp = NULL;
        }
        else
        {
          if (syntax_expression_stack_push(nodes, expr) == false)
          {
            throw("failed to push to the nodes stack");
          }
        }

        last_expr_kind = expr->kind;
        expression_destroy(expr);
        expr = NULL;
        break;

      default:
        print_token(token);
        throw(X_ERROR_UNSUPPORTED_TOKEN);
    }

    __free(token);
    token = NULL;
  }

  syntax_expression_stack_destroy(symbol_stack);
  syntax_expression_stack_destroy(nodes);

  return root;
}
