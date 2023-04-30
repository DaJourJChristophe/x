/**
 * @file eval.c
 * @author Da'Jour J. Christophe (dajour.christophe@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-04-23
 *
 * @copyright Copyright (c) 2023 Da'Jour J. Christophe. All rights reserved.
 */
#include "common.h"
#include "error.h"
#include "expr.h"
#include "facts.h"
#include "symbol-table.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Print a short-hand Expression for development purposes.
 */
void print_expr(syntax_expression_t *expr);

static syntax_expression_t *eval_calc(syntax_expression_t *root, symbol_table_t *table)
{
  if (root == NULL)
  {
    return NULL;
  }

  if (root->left  == NULL &&
      root->right == NULL)
  {
    return expression_copy(root);
  }

  syntax_expression_t *lval = eval_calc(root->left,  table);
  syntax_expression_t *rval = eval_calc(root->right, table);
  syntax_expression_t *temp = NULL;

  syntax_expression_t *retval = NULL;

  syntax_token_t token;

  switch (root->kind)
  {
    case UNARY_EXPRESSION:

      temp = lval;
      if (temp->kind == WORD_LITERAL)
      {
        token.type = NUM_TOK;
        token.size = sizeof(int);
        token.data = __malloc(sizeof(int));
        memcpy(token.data, symbol_table_get(table, temp->data, temp->size, NULL), sizeof(int));
        expression_destroy(temp);
        temp = NULL;
        lval = number_expression_new(&token);
        __free(token.data);
        token.data = NULL;
      }

      switch (root->type)
      {
        case INCREMENT_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = ++(*(int *)lval->data);
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          break;

        case DECREMENT_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = --(*(int *)lval->data);
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          break;

        case PLUS_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          break;

        case HYPHEN_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = -(*(int *)lval->data);
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          break;

        case BITWISE_TERNARY_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = ~(*(int *)lval->data);
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          break;
      }
      break;

    case ASSIGNMENT_EXPRESSION:
      symbol_table_add(table, lval->data, rval->data, lval->size, rval->size);
      expression_destroy(lval);
      expression_destroy(rval);
      break;

    case BINARY_EXPRESSION:

      temp = lval;
      if (temp->kind == WORD_LITERAL)
      {
        token.type = NUM_TOK;
        token.size = sizeof(int);
        token.data = __malloc(sizeof(int));
        memcpy(token.data, symbol_table_get(table, temp->data, temp->size, NULL), sizeof(int));
        expression_destroy(temp);
        temp = NULL;
        lval = number_expression_new(&token);
        __free(token.data);
        token.data = NULL;
      }

      temp = rval;
      if (temp->kind == WORD_LITERAL)
      {
        token.type = NUM_TOK;
        token.size = sizeof(int);
        token.data = __malloc(sizeof(int));
        memcpy(token.data, symbol_table_get(table, temp->data, temp->size, NULL), sizeof(int));
        expression_destroy(temp);
        temp = NULL;
        rval = number_expression_new(&token);
        __free(token.data);
        token.data = NULL;
      }

      switch (root->type)
      {
        case PLUS_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data + *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case EXPONENTIAL_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = (int)pow(*(int *)lval->data, *(int *)rval->data);
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case HYPHEN_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data - *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case STAR_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data * *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case SLASH_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data / *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case MODULUS_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data % *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case REMAINDER_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = (int)(*(int *)lval->data / *(int *)rval->data);
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case BITWISE_AND_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data & *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case BITWISE_OR_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data | *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case BITWISE_XOR_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data ^ *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case BITWISE_SHFT_LEFT_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data << *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case BITWISE_SHFT_RIGHT_TOK:
          token.type = NUM_TOK;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data >> *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;
      }
      break;

    default:
      throw("cannot evaluate an undefined expression");
  }

  return retval;
}

void eval(syntax_expression_t *root, symbol_table_t *table)
{
  syntax_expression_t *retval = eval_calc(root, table);
  if (retval != NULL)
  {
    printf("%d\n", *(int *)retval->data);
    expression_destroy(retval);
  }
}
