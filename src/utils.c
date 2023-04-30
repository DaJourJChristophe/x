/**
 * @file utils.c
 * @author Da'Jour J. Christophe (dajour.christophe@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-04-23
 *
 * @copyright Copyright (c) 2023 Da'Jour J. Christophe. All rights reserved.
 */
#include "error.h"
#include "expr.h"
#include "facts.h"
#include "token.h"

#include <stdio.h>

void print_expr(syntax_expression_t *expr)
{
  if (expr == NULL)
  {
    throw("null expression pointer");
  }

#ifdef VERBOSE
  if (expr->data == NULL)
  {
    fprintf(stdout, "warn(): %s\n", "null expr->data pointer");
  }
#endif/*VERBOSE*/

  switch (expr->kind)
  {
    case BINARY_EXPRESSION:
      switch (expr->type)
      {
        case PLUS_TOK:
          printf("+");
          break;

        case STAR_TOK:
          printf("*");
          break;
      }
      break;

    case NUMBER_LITERAL:
      printf("%d", *(int *)expr->data);
      break;
  }
}

void print_expression(syntax_expression_t *expr)
{
  if (expr == NULL)
  {
    throw("null expression pointer");
  }

#ifdef VERBOSE
  if (expr->data == NULL)
  {
    fprintf(stdout, "warn(): %s\n", "null expr->data pointer");
  }
#endif/*VERBOSE*/

  switch (expr->kind)
  {
    case BINARY_EXPRESSION:
      switch (expr->type)
      {
        case PLUS_TOK:
          printf("[BINARY EXPRESSION <add>]\n");
          break;

        case STAR_TOK:
          printf("[BINARY EXPRESSION <mul>]\n");
          break;
      }
      break;

    case NUMBER_LITERAL:
      printf("[NUMBER EXPRESSION <%d>]\n", *(int *)expr->data);
      break;
  }
}

void print_token(syntax_token_t *token)
{
  if (token == NULL)
  {
    throw("null token pointer");
  }

#ifdef VERBOSE
  if (token->data == NULL)
  {
    fprintf(stdout, "warn(): %s\n", "null token->data pointer");
  }
#endif/*VERBOSE*/

  switch (token->type)
  {
    case BAD_TOK:
      printf("[BAD TOKEN <%p>]\n", token->data);
      break;

    case SPACE_TOK:
      printf("[SPACE TOKEN <%p>]\n", token->data);
      break;

    case TAB_TOK:
      printf("[TAB TOKEN <%p>]\n", token->data);
      break;

    case EOL_TOK:
      printf("[EOL TOKEN <%p>]\n", token->data);
      break;

    case EOE_TOK:
      printf("[EOE TOKEN <%p>]\n", token->data);
      break;

    case EOF_TOK:
      printf("[EOF TOKEN <%p>]\n", token->data);
      break;

    case TEXT_TOK:
      printf("[TEXT TOKEN <%p>]\n", token->data);
      break;

    case NUM_TOK:
      printf("[NUMBER TOKEN <%d>]\n", *(int *)token->data);
      break;

    case WORD_TOK:
      printf("[WORD TOKEN <%p>]\n", token->data);
      break;

    case FLT_POINT_TOK:
      printf("[DECIMAL TOKEN <%p>]\n", token->data);
      break;

    case PLUS_TOK:
      printf("[ADD TOKEN <%p>]\n", token->data);
      break;

    case SLASH_TOK:
      printf("[DIVISION TOKEN <%p>]\n", token->data);
      break;

    case HYPHEN_TOK:
      printf("[SUBTRACTION TOKEN <%p>]\n", token->data);
      break;

    case EQUAL_TOK:
      printf("[EQUAL TOKEN <%p>]\n", token->data);
      break;

    case MODULUS_TOK:
      printf("[MODULUS TOKEN <%p>]\n", token->data);
      break;

    case BITWISE_AND_TOK:
      printf("[BITWISE AND TOKEN <%p>]\n", token->data);
      break;

    case BITWISE_XOR_TOK:
      printf("[BITWISE XOR TOKEN <%p>]\n", token->data);
      break;

    case BITWISE_OR_TOK:
      printf("[BITWISE OR TOKEN <%p>]\n", token->data);
      break;

    case BITWISE_TERNARY_TOK:
      printf("[BITWISE TERNARY TOKEN <%p>]\n", token->data);
      break;

    case COND_OR_TOK:
      printf("[CONDITIONAL OR TOKEN <%p>]\n", token->data);
      break;

    case COND_AND_TOK:
      printf("[CONDITIONAL AND TOKEN <%p>]\n", token->data);
      break;

    case INTEGER_TOK:
      printf("[INTEGER RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case FLOAT_TOK:
      printf("[FLOAT RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case DOUBLE_TOK:
      printf("[DOUBLE RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case STRING_TOK:
      printf("[STRING RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case CLASS_TOK:
      printf("[CLASS RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case OBJECT_TOK:
      printf("[OBJECT RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case MATRIX_TOK:
      printf("[MATRIX RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case SCALAR_TOK:
      printf("[SCALAR RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case VECTOR_TOK:
      printf("[VECTOR RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case VOID_TOK:
      printf("[VOID RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case OPEN_CURLY_BRACKET_TOK:
      printf("[OPEN_CURLY_BRACKET TOKEN <%p>]\n", token->data);
      break;

    case OPEN_PARENTHESIS_TOK:
      printf("[OPEN_PARENTHESIS TOKEN <%p>]\n", token->data);
      break;

    case OPEN_SQUARE_BRACKET_TOK:
      printf("[OPEN_SQUARE_BRACKET TOKEN <%p>]\n", token->data);
      break;

    case CLOSE_CURLY_BRACKET_TOK:
      printf("[CLOSE_CURLY_BRACKET TOKEN <%p>]\n", token->data);
      break;

    case CLOSE_PARENTHESIS_TOK:
      printf("[CLOSE_PARENTHESIS TOKEN <%p>]\n", token->data);
      break;

    case CLOSE_SQUARE_BRACKET_TOK:
      printf("[CLOSE_SQUARE_BRACKET TOKEN <%p>]\n", token->data);
      break;

    case RETURN_TOK:
      printf("[RETURN RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case COMMA_TOK:
      printf("[COMMA TOKEN <%p>]\n", token->data);
      break;

    case COLON_TOK:
      printf("[COLON TOKEN <%p>]\n", token->data);
      break;

    case PUBLIC_TOK:
      printf("[PUBLIC RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case PRIVATE_TOK:
      printf("[PRIVATE RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case PROTECTED_TOK:
      printf("[PROTECTED RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case STATIC_TOK:
      printf("[STATIC RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case CONST_TOK:
      printf("[CONST RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case NIL_TOK:
      printf("[NULL RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case PRINT_TOK:
      printf("[PRINT RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case SWITCH_TOK:
      printf("[SWITCH RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case CASE_TOK:
      printf("[CASE RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case BREAK_TOK:
      printf("[BREAK RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case DEFAULT_TOK:
      printf("[DEFAULT RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case FOR_TOK:
      printf("[FOR RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case IF_TOK:
      printf("[IF RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case WHILE_TOK:
      printf("[WHILE RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case DOT_TOK:
      printf("[DOT TOKEN <%p>]\n", token->data);
      break;

    case STAR_TOK:
      printf("[STAR TOKEN <%p>]\n", token->data);
      break;

    case LEFT_CARET_TOK:
      printf("[LEFT CARET TOKEN <%p>]\n", token->data);
      break;

    case RIGHT_CARET_TOK:
      printf("[RIGHT CARET TOKEN <%p>]\n", token->data);
      break;

    case COND_EQUALS_TOK:
      printf("[EQUALS TOKEN <%p>]\n", token->data);
      break;

    case INCREMENT_TOK:
      printf("[INCREMENT TOKEN <%p>]\n", token->data);
      break;

    case DECREMENT_TOK:
      printf("[DECREMENT TOKEN <%p>]\n", token->data);
      break;

    case EXPONENTIAL_TOK:
      printf("[EXPONENTIAL TOKEN <%p>]\n", token->data);
      break;

    case REMAINDER_TOK:
      printf("[REMAINDER TOKEN <%p>]\n", token->data);
      break;

    case ABSTRACT_TOK:
      printf("[ABSTRACT RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case ANNOTATION_TOK:
      printf("[ANNOTATION TOKEN <%p>]\n", token->data);
      break;

    case IMPORT_TOK:
      printf("[IMPORT RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case PACKAGE_TOK:
      printf("[PACKAGE RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case EXPORT_TOK:
      printf("[EXPORT RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case BOOLEAN_TOK:
      printf("[BOOLEAN RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case TRUE_TOK:
      printf("[TRUE RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case FALSE_TOK:
      printf("[FALSE RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case IS_TOK:
      printf("[IS RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case UNLESS_TOK:
      printf("[UNLESS RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case SET_TOK:
      printf("[SET RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case IMMUTABLE_TOK:
      printf("[IMMUTABLE RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case YIELD_TOK:
      printf("[YIELD RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case LAMBDA_TOK:
      printf("[LAMBDA TOKEN <%p>]\n", token->data);
      break;
  }
}
