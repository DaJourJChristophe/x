#include "token.h"

#include <stdio.h>

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

void print_token(syntax_token_t *token)
{
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
