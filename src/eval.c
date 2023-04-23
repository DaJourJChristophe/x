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
      switch (root->type)
      {
        case SUBTRACTION:
          token.type = NUMBER;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = -(*(int *)lval->data);
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          break;

        case BITWISE_TERNARY:
          token.type = NUMBER;
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
      if (temp->kind == WORD_EXPRESSION)
      {
        token.type = NUMBER;
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
      if (temp->kind == WORD_EXPRESSION)
      {
        token.type = NUMBER;
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
        case ADDITION:
          token.type = NUMBER;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data + *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case EXPONENTIAL:
          token.type = NUMBER;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = (int)pow(*(int *)lval->data, *(int *)rval->data);
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case SUBTRACTION:
          token.type = NUMBER;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data - *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case STAR:
          token.type = NUMBER;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data * *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case DIVISION:
          token.type = NUMBER;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data / *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case MODULUS:
          token.type = NUMBER;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data % *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case REMAINDER:
          token.type = NUMBER;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = (int)(*(int *)lval->data / *(int *)rval->data);
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case BITWISE_AND:
          token.type = NUMBER;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data & *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case BITWISE_OR:
          token.type = NUMBER;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data | *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case BITWISE_XOR:
          token.type = NUMBER;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data ^ *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case BITWISE_SHIFT_LEFT:
          token.type = NUMBER;
          token.size = sizeof(int);
          token.data = __malloc(sizeof(int));
          *(int *)token.data = *(int *)lval->data << *(int *)rval->data;
          retval = number_expression_new(&token);
          __free(token.data);
          token.data = NULL;
          expression_destroy(lval);
          expression_destroy(rval);
          break;

        case BITWISE_SHIFT_RIGHT:
          token.type = NUMBER;
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
