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

struct token
{
  int type;

  const char *i;   /* start of match */
  const char *j;   /*   end of match */
};

/**
 * @brief Define a namespace for the token structure.
 */
typedef struct token token_t;

/**
 * @brief Define a namespace for the syntax token structure.
 */
typedef token_t syntax_token_t;

void print_token(syntax_token_t *token)
{
  switch (token->type)
  {
    case UNDEFINED:
      printf("[UNDEFINED TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case SPACE:
      printf("[SPACE TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case TAB:
      printf("[TAB TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case EOL:
      printf("[EOL TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case EOE:
      printf("[EOE TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case SYMBOL:
      printf("[SYMBOL TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case LEXER_EOF:
      printf("[EOF TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case TEXT:
      printf("[TEXT TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case NUMBER:
      printf("[NUMBER TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case WORD:
      printf("[WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case DECIMAL:
      printf("[DECIMAL TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case ADDITION:
      printf("[ADD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case DIVISION:
      printf("[DIVISION TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case SUBTRACTION:
      printf("[SUBTRACTION TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case EQUAL:
      printf("[EQUAL TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case MODULUS:
      printf("[MODULUS TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case BITWISE_AND:
      printf("[BITWISE AND TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case BITWISE_XOR:
      printf("[BITWISE XOR TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case BITWISE_OR:
      printf("[BITWISE OR TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case BITWISE_TERNARY:
      printf("[BITWISE TERNARY TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case CONDITIONAL_OR:
      printf("[CONDITIONAL OR TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case CONDITIONAL_AND:
      printf("[CONDITIONAL AND TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case INTEGER:
      printf("[INTEGER RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case FLOAT:
      printf("[FLOAT RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case DOUBLE:
      printf("[DOUBLE RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case STRING:
      printf("[STRING RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case CLASS:
      printf("[CLASS RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case OBJECT:
      printf("[OBJECT RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case MATRIX:
      printf("[MATRIX RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case SCALAR:
      printf("[SCALAR RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case VECTOR:
      printf("[VECTOR RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case VOID:
      printf("[VOID RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case OPEN_CURLY_BRACKET:
      printf("[OPEN_CURLY_BRACKET TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case OPEN_PARENTHESIS:
      printf("[OPEN_PARENTHESIS TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case OPEN_SQUARE_BRACKET:
      printf("[OPEN_SQUARE_BRACKET TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case CLOSE_CURLY_BRACKET:
      printf("[CLOSE_CURLY_BRACKET TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case CLOSE_PARENTHESIS:
      printf("[CLOSE_PARENTHESIS TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case CLOSE_SQUARE_BRACKET:
      printf("[CLOSE_SQUARE_BRACKET TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case RETURN:
      printf("[RETURN RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case COMMA:
      printf("[COMMA TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case COLON:
      printf("[COLON TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case PUBLIC:
      printf("[PUBLIC RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case PRIVATE:
      printf("[PRIVATE RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case PROTECTED:
      printf("[PROTECTED RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case STATIC:
      printf("[STATIC RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case CONST:
      printf("[CONST RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case NIL:
      printf("[NULL RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case PRINT:
      printf("[PRINT RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case SWITCH:
      printf("[SWITCH RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case CASE:
      printf("[CASE RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case BREAK:
      printf("[BREAK RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case DEFAULT:
      printf("[DEFAULT RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case FOR:
      printf("[FOR RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case IF:
      printf("[IF RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case WHILE:
      printf("[WHILE RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case DOT:
      printf("[DOT TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case STAR:
      printf("[STAR TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case LEFT_CARET:
      printf("[LEFT CARET TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case RIGHT_CARET:
      printf("[RIGHT CARET TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case EQUALS:
      printf("[EQUALS TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case INCREMENT:
      printf("[INCREMENT TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case DECREMENT:
      printf("[DECREMENT TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case EXPONENTIAL:
      printf("[EXPONENTIAL TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case REMAINDER:
      printf("[REMAINDER TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case ABSTRACT:
      printf("[ABSTRACT RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case ANNOTATION:
      printf("[ANNOTATION TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case IMPORT:
      printf("[IMPORT RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case PACKAGE:
      printf("[PACKAGE RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case EXPORT:
      printf("[EXPORT RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case BOOLEAN:
      printf("[BOOLEAN RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case TRUE:
      printf("[TRUE RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case FALSE:
      printf("[FALSE RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case IS:
      printf("[IS RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case UNLESS:
      printf("[UNLESS RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case SET:
      printf("[SET RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case IMMUTABLE:
      printf("[IMMUTABLE RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case YIELD:
      printf("[YIELD RESERVED WORD TOKEN <%p:%p>]\n", token->i, token->j);
      break;

    case LAMBDA:
      printf("[LAMBDA TOKEN <%p:%p>]\n", token->i, token->j);
      break;
  }
}
