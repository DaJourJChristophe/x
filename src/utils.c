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
        case ADDITION:
          printf("+");
          break;

        case STAR:
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
        case ADDITION:
          printf("[BINARY EXPRESSION <add>]\n");
          break;

        case STAR:
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
    case UNDEFINED:
      printf("[UNDEFINED TOKEN <%p>]\n", token->data);
      break;

    case SPACE:
      printf("[SPACE TOKEN <%p>]\n", token->data);
      break;

    case TAB:
      printf("[TAB TOKEN <%p>]\n", token->data);
      break;

    case EOL:
      printf("[EOL TOKEN <%p>]\n", token->data);
      break;

    case EOE:
      printf("[EOE TOKEN <%p>]\n", token->data);
      break;

    case SYMBOL:
      printf("[SYMBOL TOKEN <%p>]\n", token->data);
      break;

    case LEXER_EOF:
      printf("[EOF TOKEN <%p>]\n", token->data);
      break;

    case TEXT:
      printf("[TEXT TOKEN <%p>]\n", token->data);
      break;

    case NUMBER:
      printf("[NUMBER TOKEN <%d>]\n", *(int *)token->data);
      break;

    case WORD:
      printf("[WORD TOKEN <%p>]\n", token->data);
      break;

    case DECIMAL:
      printf("[DECIMAL TOKEN <%p>]\n", token->data);
      break;

    case ADDITION:
      printf("[ADD TOKEN <%p>]\n", token->data);
      break;

    case DIVISION:
      printf("[DIVISION TOKEN <%p>]\n", token->data);
      break;

    case SUBTRACTION:
      printf("[SUBTRACTION TOKEN <%p>]\n", token->data);
      break;

    case EQUAL:
      printf("[EQUAL TOKEN <%p>]\n", token->data);
      break;

    case MODULUS:
      printf("[MODULUS TOKEN <%p>]\n", token->data);
      break;

    case BITWISE_AND:
      printf("[BITWISE AND TOKEN <%p>]\n", token->data);
      break;

    case BITWISE_XOR:
      printf("[BITWISE XOR TOKEN <%p>]\n", token->data);
      break;

    case BITWISE_OR:
      printf("[BITWISE OR TOKEN <%p>]\n", token->data);
      break;

    case BITWISE_TERNARY:
      printf("[BITWISE TERNARY TOKEN <%p>]\n", token->data);
      break;

    case CONDITIONAL_OR:
      printf("[CONDITIONAL OR TOKEN <%p>]\n", token->data);
      break;

    case CONDITIONAL_AND:
      printf("[CONDITIONAL AND TOKEN <%p>]\n", token->data);
      break;

    case INTEGER:
      printf("[INTEGER RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case FLOAT:
      printf("[FLOAT RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case DOUBLE:
      printf("[DOUBLE RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case STRING:
      printf("[STRING RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case CLASS:
      printf("[CLASS RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case OBJECT:
      printf("[OBJECT RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case MATRIX:
      printf("[MATRIX RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case SCALAR:
      printf("[SCALAR RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case VECTOR:
      printf("[VECTOR RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case VOID:
      printf("[VOID RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case OPEN_CURLY_BRACKET:
      printf("[OPEN_CURLY_BRACKET TOKEN <%p>]\n", token->data);
      break;

    case OPEN_PARENTHESIS:
      printf("[OPEN_PARENTHESIS TOKEN <%p>]\n", token->data);
      break;

    case OPEN_SQUARE_BRACKET:
      printf("[OPEN_SQUARE_BRACKET TOKEN <%p>]\n", token->data);
      break;

    case CLOSE_CURLY_BRACKET:
      printf("[CLOSE_CURLY_BRACKET TOKEN <%p>]\n", token->data);
      break;

    case CLOSE_PARENTHESIS:
      printf("[CLOSE_PARENTHESIS TOKEN <%p>]\n", token->data);
      break;

    case CLOSE_SQUARE_BRACKET:
      printf("[CLOSE_SQUARE_BRACKET TOKEN <%p>]\n", token->data);
      break;

    case RETURN:
      printf("[RETURN RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case COMMA:
      printf("[COMMA TOKEN <%p>]\n", token->data);
      break;

    case COLON:
      printf("[COLON TOKEN <%p>]\n", token->data);
      break;

    case PUBLIC:
      printf("[PUBLIC RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case PRIVATE:
      printf("[PRIVATE RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case PROTECTED:
      printf("[PROTECTED RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case STATIC:
      printf("[STATIC RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case CONST:
      printf("[CONST RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case NIL:
      printf("[NULL RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case PRINT:
      printf("[PRINT RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case SWITCH:
      printf("[SWITCH RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case CASE:
      printf("[CASE RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case BREAK:
      printf("[BREAK RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case DEFAULT:
      printf("[DEFAULT RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case FOR:
      printf("[FOR RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case IF:
      printf("[IF RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case WHILE:
      printf("[WHILE RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case DOT:
      printf("[DOT TOKEN <%p>]\n", token->data);
      break;

    case STAR:
      printf("[STAR TOKEN <%p>]\n", token->data);
      break;

    case LEFT_CARET:
      printf("[LEFT CARET TOKEN <%p>]\n", token->data);
      break;

    case RIGHT_CARET:
      printf("[RIGHT CARET TOKEN <%p>]\n", token->data);
      break;

    case EQUALS:
      printf("[EQUALS TOKEN <%p>]\n", token->data);
      break;

    case INCREMENT:
      printf("[INCREMENT TOKEN <%p>]\n", token->data);
      break;

    case DECREMENT:
      printf("[DECREMENT TOKEN <%p>]\n", token->data);
      break;

    case EXPONENTIAL:
      printf("[EXPONENTIAL TOKEN <%p>]\n", token->data);
      break;

    case REMAINDER:
      printf("[REMAINDER TOKEN <%p>]\n", token->data);
      break;

    case ABSTRACT:
      printf("[ABSTRACT RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case ANNOTATION:
      printf("[ANNOTATION TOKEN <%p>]\n", token->data);
      break;

    case IMPORT:
      printf("[IMPORT RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case PACKAGE:
      printf("[PACKAGE RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case EXPORT:
      printf("[EXPORT RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case BOOLEAN:
      printf("[BOOLEAN RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case TRUE:
      printf("[TRUE RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case FALSE:
      printf("[FALSE RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case IS:
      printf("[IS RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case UNLESS:
      printf("[UNLESS RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case SET:
      printf("[SET RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case IMMUTABLE:
      printf("[IMMUTABLE RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case YIELD:
      printf("[YIELD RESERVED WORD TOKEN <%p>]\n", token->data);
      break;

    case LAMBDA:
      printf("[LAMBDA TOKEN <%p>]\n", token->data);
      break;
  }
}
