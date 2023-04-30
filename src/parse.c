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
      case EOF_TOK:
        // print_token(token);
        break;

      case EOE_TOK:
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

      case EOL_TOK:
      case TAB_TOK:
      case SPACE_TOK:
        // print_token(token);
        break;

      case CLOSE_PARENTHESIS_TOK:
        while ((temp = syntax_expression_stack_pop(symbol_stack)) != NULL)
        {
          if (temp->type == OPEN_PARENTHESIS_TOK)
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

      case OPEN_PARENTHESIS_TOK:
        expr = binary_expression_new(token, NULL, NULL);

        if (syntax_expression_stack_push(symbol_stack, expr) == false)
        {
          throw("failed to push to the nodes stack");
        }

        last_expr_kind = expr->kind;
        expression_destroy(expr);
        expr = NULL;
        break;

      case EQUAL_TOK:
        expr = assignment_expression_new(token, NULL, NULL);

        if (syntax_expression_stack_push(symbol_stack, expr) == false)
        {
          throw("failed to push to the symbol stack");
        }

        last_expr_kind = expr->kind;
        expression_destroy(expr);
        expr = NULL;
        break;

      case BOOLEAN_TOK:
      case INTEGER_TOK:
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

      case WORD_TOK:
        temp = syntax_expression_stack_peek(declarations);

        if (temp != NULL)
        {
          temp->data = token->data;
          temp->size = token->size;

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

      case NIL_TOK:
        expr = nil_literal_new(token);

        if (syntax_expression_stack_push(nodes, expr) == false)
        {
          throw("failed to push to the nodes stack");
        }

        last_expr_kind = expr->kind;
        expression_destroy(expr);
        expr = NULL;
        break;

      case TRUE_TOK:
      case FALSE_TOK:
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

      case DECREMENT_TOK:
      case INCREMENT_TOK:
        expr = unary_expression_new(token);

        if (syntax_expression_stack_push(symbol_stack, expr) == false)
        {
          throw("failed to push to the symbol stack");
        }

        last_expr_kind = expr->kind;
        expression_destroy(expr);
        expr = NULL;
        break;

      case PLUS_TOK:
      case SLASH_TOK:
      case EXPONENTIAL_TOK:
      case MODULUS_TOK:
      case REMAINDER_TOK:
      case HYPHEN_TOK:
      case STAR_TOK:

      case BITWISE_AND_TOK:
      case BITWISE_OR_TOK:
      case BITWISE_TERNARY_TOK:
      case BITWISE_XOR_TOK:
      case BITWISE_SHFT_LEFT_TOK:
      case BITWISE_SHFT_RIGHT_TOK:

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

      case INT_TOK:
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

      case NUM_TOK:
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
