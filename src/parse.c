/**
 * @file parse.c
 * @author Da'Jour J. Christophe (dajour.christophe@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-04-23
 *
 * @copyright Copyright (c) 2023 Da'Jour J. Christophe. All rights reserved.
 */
#include "common.h"
#include "bind.h"
#include "diagnostic.h"
#include "lexer.h"
#include "token.h"
#include "cache.h"
#include "expr.h"
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
 * @brief
 */
diagnostic_t *diagnostics = NULL;

/**
 * @brief
 */
syntax_expression_t *parse(syntax_queue_t *queue)
{
  syntax_token_t *token = NULL;

  syntax_expression_t *expr = NULL;
  syntax_expression_t *temp = NULL;
  syntax_expression_t *tamp = NULL;
  syntax_expression_t *root = NULL;

  syntax_expression_stack_t *symbol_stack = syntax_expression_stack_new(64);
  syntax_expression_stack_t *nodes = syntax_expression_stack_new(64);
  syntax_expression_stack_t *declarations = syntax_expression_stack_new(64);

  diagnostics = diagnostic_new();

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

            case ASSIGNMENT_EXPRESSION:
              temp->right = syntax_expression_stack_pop(nodes);
              temp->left  = syntax_expression_stack_pop(nodes);

              temp = bound_assignment_expression_new(temp);

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

      case EQUAL:
        expr = assignment_expression_new(token, NULL, NULL);

        if (syntax_expression_stack_push(symbol_stack, expr) == false)
        {
          throw("failed to push to the symbol stack");
        }

        last_expr_kind = expr->kind;
        expression_destroy(expr);
        expr = NULL;
        break;

      case BOOLEAN:
      case INTEGER:
        expr = declaration_expression_new(token);
        expr = bound_declaration_expression_new(expr);

        if (syntax_expression_stack_push(declarations, expr) == false)
        {
          throw("failed to push to the nodes stack");
        }

        last_expr_kind = expr->kind;
        expression_destroy(expr);
        expr = NULL;
        break;

      case WORD:
        temp = syntax_expression_stack_peek(declarations);

        if (temp != NULL)
        {
          temp->data = token->data;

          if (syntax_expression_stack_push(nodes, temp) == false)
          {
            throw("failed to push to the nodes stack");
          }

          expression_destroy(syntax_expression_stack_pop(declarations));
          last_expr_kind = temp->kind;
          expression_destroy(temp);
          temp = NULL;
        }
        else
        {
          expr = word_expression_new(token);

          if (syntax_expression_stack_push(nodes, expr) == false)
          {
            throw("failed to push to the nodes stack");
          }

          last_expr_kind = expr->kind;
          expression_destroy(expr);
          expr = NULL;
        }
        break;

      case NIL:
        expr = nil_literal_new(token);

        if (syntax_expression_stack_push(nodes, expr) == false)
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
        expr = bound_boolean_expression_new(expr);

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

      case INTEGER_VALUE:
        expr = integer_expression_new(token);
        temp = syntax_expression_stack_peek(symbol_stack);

        expr = bound_integer_expression_new(expr);

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

      case NUMBER:
        expr = number_expression_new(token);
        temp = syntax_expression_stack_peek(symbol_stack);

        expr = bound_number_expression_new(expr);

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
  syntax_expression_stack_destroy(declarations);

  return ((diagnostic_is_empty(diagnostics)) == true ? root : NULL);
}
