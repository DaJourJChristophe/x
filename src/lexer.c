/**
 * @file lexer.c
 * @author Da'Jour J. Christophe (dajour.christophe@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-04-08
 *
 * @copyright Copyright (c) 2023 Da'Jour J. Christophe. All rights reserved.
 */
#include "common.h"
#include "error.h"
#include "ae.h"
#include "io.h"
#include "re.h"
#include "lexer.h"
#include "queue.h"
#include "token.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUF      (1 << 20)
#define MAX_QUEUE   1024

#define LEXER_HASH_INIT   5381

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

/**
 * @brief Reserved Words.
 */
enum
{
  ABSTRACT_RESERVED_WORD = 7572149969927417,
  BOOLEAN_RESERVED_WORD = 229460885180549,
  BREAK_RESERVED_WORD = 210707980106,
  CASE_RESERVED_WORD = 6385108193,
  CLASS_RESERVED_WORD = 210708946651,
  CONST_RESERVED_WORD = 210709068620,
  DEFAULT_RESERVED_WORD = 229463065711754,
  DOUBLE_RESERVED_WORD = 6953438632736,
  EXPORT_RESERVED_WORD = 6953488276103,
  FALSE_RESERVED_WORD = 210712121072,
  FLOAT_RESERVED_WORD = 210712519067,
  FOR_RESERVED_WORD = 193491852,
  IF_RESERVED_WORD = 5863476,
  IMMUTABLE_RESERVED_WORD = 249892661372121605,
  IMPORT_RESERVED_WORD = 6953631772544,
  INTEGER_RESERVED_WORD = 193495088,
  IS_RESERVED_WORD = 5863489,
  MATRIX_RESERVED_WORD = 6953774229786,
  NIL_RESERVED_WORD = 6385525056,
  OBJECT_RESERVED_WORD = 6953853312764,
  PACKAGE_RESERVED_WORD = 229478403565457,
  PRINT_RESERVED_WORD = 210724587794,
  PRIVATE_RESERVED_WORD = 229479076378400,
  PROTECTED_RESERVED_WORD = 249902721850767151,
  PUBLIC_RESERVED_WORD = 6953914700964,
  RETURN_RESERVED_WORD = 6953974653989,
  SCALAR_RESERVED_WORD = 6954010724379,
  SET_RESERVED_WORD = 193505681,
  STATIC_RESERVED_WORD = 6954030893997,
  STRING_RESERVED_WORD = 6954031493116,
  SWITCH_RESERVED_WORD = 6954034739063,
  TRUE_RESERVED_WORD = 6385737701,
  UNLESS_RESERVED_WORD = 6954102428575,
  VECTOR_RESERVED_WORD = 6954130583448,
  VOID_RESERVED_WORD = 6385805911,
  WHILE_RESERVED_WORD = 210732529790,
  YIELD_RESERVED_WORD = 210734933212,
};

/**
 * @brief Hash a string, based on the djb2 algorithm, return the hash
 *        via a pointer parameter. This algorithm (k=33) was first
 *        reported by Dan Bernstein many years ago in comp.lang.c.
 *        another version of this algorithm (now favored by bernstein)
 *        uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of
 *        number 33 (why it works better than many other constants,
 *        prime or not) has never been adequately explained.
 */
static void hash(uint64_t *hash, const char **s)
{
  for (; ae_match(**s, AE_IS_ALPHA); *s += 1)
  {
    *hash = (*hash << 5) + *hash + **s;
  }
}

#define INLINE_VOID_T inline void always_inline

static const int whitespace_syntax_tokens[128] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0,
  TAB,
  EOL,
  0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  SPACE,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
};

static INLINE_VOID_T handle_whitespace(syntax_queue_t *queue, syntax_token_t *token, const char **data)
{
  do
  {
    int index = **data;
    token->type = whitespace_syntax_tokens[index];
    *data += 1;
    token->j = *data;

    if (syntax_queue_write(queue, token) == false)
    {
      die(X_ERROR_SYNTAX_QUEUE_WRITE, __FILE__, __func__);
    }
  }
  while (ae_match(**data, AE_IS_WHITE));
}

static INLINE_VOID_T handle_number(syntax_queue_t *queue, syntax_token_t *token, const char **data)
{
  token->type = NUMBER;
  token->i = *data;

  do
  {
    *data += 1;

    if (**data == '.')
    {
      token->type = DECIMAL;
      *data += 1;
    }
  }
  while (ae_match(**data, AE_IS_DIGIT));

  token->j = *data;

  if (syntax_queue_write(queue, token) == false)
  {
    die(X_ERROR_SYNTAX_QUEUE_WRITE, __FILE__, __func__);
  }
}

static INLINE_VOID_T handle_word(syntax_queue_t *queue, syntax_token_t *token, const char **data)
{
  uint64_t _hash = LEXER_HASH_INIT;

  token->type = WORD;
  token->i = *data;

  hash(&_hash, data);

  switch (_hash)
  {
    case ABSTRACT_RESERVED_WORD:
      token->type = ABSTRACT;
      break;

    case BOOLEAN_RESERVED_WORD:
      token->type = BOOLEAN;
      break;

    case BREAK_RESERVED_WORD:
      token->type = BREAK;
      break;

    case CASE_RESERVED_WORD:
      token->type = CASE;
      break;

    case CLASS_RESERVED_WORD:
      token->type = CLASS;
      break;

    case CONST_RESERVED_WORD:
      token->type = CONST;
      break;

    case DEFAULT_RESERVED_WORD:
      token->type = DEFAULT;
      break;

    case DOUBLE_RESERVED_WORD:
      token->type = DOUBLE;
      break;

    case EXPORT_RESERVED_WORD:
      token->type = EXPORT;
      break;

    case FALSE_RESERVED_WORD:
      token->type = FALSE;
      break;

    case FLOAT_RESERVED_WORD:
      token->type = FLOAT;
      break;

    case FOR_RESERVED_WORD:
      token->type = FOR;
      break;

    case IF_RESERVED_WORD:
      token->type = IF;
      break;

    case IMMUTABLE_RESERVED_WORD:
      token->type = IMMUTABLE;
      break;

    case IMPORT_RESERVED_WORD:
      token->type = IMPORT;
      break;

    case INTEGER_RESERVED_WORD:
      token->type = INTEGER;
      break;

    case IS_RESERVED_WORD:
      token->type = IS;
      break;

    case MATRIX_RESERVED_WORD:
      token->type = MATRIX;
      break;

    case NIL_RESERVED_WORD:
      token->type = NIL;
      break;

    case OBJECT_RESERVED_WORD:
      token->type = OBJECT;
      break;

    case PACKAGE_RESERVED_WORD:
      token->type = PACKAGE;
      break;

    case PRINT_RESERVED_WORD:
      token->type = PRINT;
      break;

    case PRIVATE_RESERVED_WORD:
      token->type = PRIVATE;
      break;

    case PROTECTED_RESERVED_WORD:
      token->type = PROTECTED;
      break;

    case PUBLIC_RESERVED_WORD:
      token->type = PUBLIC;
      break;

    case RETURN_RESERVED_WORD:
      token->type = RETURN;
      break;

    case SCALAR_RESERVED_WORD:
      token->type = SCALAR;
      break;

    case SET_RESERVED_WORD:
      token->type = SET;
      break;

    case STATIC_RESERVED_WORD:
      token->type = STATIC;
      break;

    case STRING_RESERVED_WORD:
      token->type = STRING;
      break;

    case SWITCH_RESERVED_WORD:
      token->type = SWITCH;
      break;

    case TRUE_RESERVED_WORD:
      token->type = TRUE;
      break;

    case UNLESS_RESERVED_WORD:
      token->type = UNLESS;
      break;

    case VECTOR_RESERVED_WORD:
      token->type = VECTOR;
      break;

    case VOID_RESERVED_WORD:
      token->type = VOID;
      break;

    case WHILE_RESERVED_WORD:
      token->type = WHILE;
      break;

    case YIELD_RESERVED_WORD:
      token->type = YIELD;
      break;
  }

  token->j = *data;

  if (syntax_queue_write(queue, token) == false)
  {
    die(X_ERROR_SYNTAX_QUEUE_WRITE, __FILE__, __func__);
  }
}

typedef void (*symbol_handler_t)(syntax_token_t *, const char **);

static void handle_symbol_single_quote(syntax_token_t *token, const char **data)
{
  token->type = TEXT;
  token->i = *data;

  do
  {
    *data += 1;
  }
  while (**data != 0 && **data != '\'');
}

static void handle_symbol_double_quote(syntax_token_t *token, const char **data)
{
  token->type = TEXT;
  token->i = *data;

  do
  {
    *data += 1;
  }
  while (**data != 0 && **data != '"');
}

static void handle_symbol_equal_sign(syntax_token_t *token, const char **data)
{
  if (*(*data + 1) == '>')
  {
    *data += 1;

    token->type = LAMBDA;
    token->i = *data;
  }
  else if (*(*data + 1) == '=')
  {
    *data += 1;

    token->type = EQUALS;
    token->i = *data;
  }
  else
  {
    token->type = EQUAL;
    token->i = *data;
  }
}

static void handle_symbol_plus_sign(syntax_token_t *token, const char **data)
{
  if (*(*data + 1) == '+')
  {
    *data += 1;

    token->type = INCREMENT;
    token->i = *data;
  }
  else
  {
    token->type = ADDITION;
    token->i = *data;
  }
}

static void handle_symbol_minus_sign(syntax_token_t *token, const char **data)
{
  if (*(*data + 1) == '-')
  {
    *data += 1;

    token->type = DECREMENT;
    token->i = *data;
  }
  else
  {
    token->type = SUBTRACTION;
    token->i = *data;
  }
}

static void handle_symbol_forward_slash(syntax_token_t *token, const char **data)
{
  if (*(*data + 1) == '/')
  {
    do
    {
      *data += 1;
    }
    while (**data != 0 && **data != '\n');
  }
  else if (*(*data + 1) == '*')
  {
    for (; *(*data + 1); *data += 1)
    {
      if (**data == '*' && *(*data + 1) == '/')
      {
        *data += 1;
        break;
      }
    }
  }
  else
  {
    token->type = DIVISION;
    token->i = *data;
  }
}

static void handle_symbol_star(syntax_token_t *token, const char **data)
{
  if (*(*data + 1) == '*')
  {
    *data += 1;

    token->type = EXPONENTIAL;
    token->i = *data;
  }
  else
  {
    token->type = STAR;
    token->i = *data;
  }
}

static void handle_symbol_vertical_bar(syntax_token_t *token, const char **data)
{
  if (*(*data + 1) == '|')
  {
    *data += 1;

    token->type = CONDITIONAL_OR;
    token->i = *data;
  }
  else
  {
    token->type = BITWISE_OR;
    token->i = *data;
  }
}

static void handle_symbol_ampersand(syntax_token_t *token, const char **data)
{
  if (*(*data + 1) == '&')
  {
    *data += 1;

    token->type = CONDITIONAL_AND;
    token->i = *data;
  }
  else
  {
    token->type = BITWISE_AND;
    token->i = *data;
  }
}

static const symbol_handler_t multiple_symbol_handlers[128] = {
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL,
  &handle_symbol_double_quote,
  NULL, NULL, NULL,
  &handle_symbol_ampersand,
  &handle_symbol_single_quote,
  NULL, NULL,
  &handle_symbol_star,
  &handle_symbol_plus_sign,
  NULL,
  &handle_symbol_minus_sign,
  NULL,
  &handle_symbol_forward_slash,
  NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL,
  &handle_symbol_equal_sign,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL,
  &handle_symbol_vertical_bar,
  NULL, NULL, NULL,
};

static const int symbol_syntax_tokens[128] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  REMAINDER,
  0,
  MODULUS,
  0, 0,
  OPEN_PARENTHESIS,
  CLOSE_PARENTHESIS,
  0, 0,
  COMMA,
  0,
  DOT,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  COLON,
  EOE,
  LEFT_CARET,
  0,
  RIGHT_CARET,
  0,
  ANNOTATION,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  OPEN_SQUARE_BRACKET,
  0,
  CLOSE_SQUARE_BRACKET,
  BITWISE_XOR,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0,
  OPEN_CURLY_BRACKET,
  0,
  CLOSE_CURLY_BRACKET,
  BITWISE_TERNARY,
  0,
};

static INLINE_VOID_T handle_symbol(syntax_queue_t *queue, syntax_token_t *token, const char **data)
{
  int index = **data;
  symbol_handler_t multiple_handler = NULL;

  multiple_handler = multiple_symbol_handlers[index];

  if (multiple_handler != NULL)
  {
    multiple_handler(token, data);
  }
  else
  {
    const int probable_token = symbol_syntax_tokens[index];

    if (probable_token != 0)
    {
      token->type = probable_token;
    }
    else
    {
      token->type = SYMBOL;
    }

    token->i = *data;
  }

  *data += 1;

  token->j = *data;

  if (syntax_queue_write(queue, token) == false)
  {
    die(X_ERROR_SYNTAX_QUEUE_WRITE, __FILE__, __func__);
  }
}

static INLINE_VOID_T handle_end_of_file(syntax_queue_t *queue, syntax_token_t *token, const char **data)
{
  token->type = LEXER_EOF;
  token->data = NULL;

  token->i = *data;
  token->j = *data;

  if (syntax_queue_write(queue, token) == false)
  {
    die(X_ERROR_SYNTAX_QUEUE_WRITE, __FILE__, __func__);
  }
}

static syntax_queue_t *parse(const char *data)
{
  syntax_queue_t *queue = NULL;
  syntax_token_t token;

  queue = syntax_queue_new(MAX_QUEUE);

  do
  {
    if (ae_match(*data, AE_IS_WHITE))
    {
      handle_whitespace(queue, &token, &data);
    }

    else if (ae_match(*data, AE_IS_DIGIT))
    {
      handle_number(queue, &token, &data);
    }

    else if (ae_match(*data, AE_IS_ALNUM))
    {
      handle_word(queue, &token, &data);
    }

    else if (ae_match(*data, AE_IS_SYMBL))
    {
      handle_symbol(queue, &token, &data);
    }

    else
    {
      throw(X_ERROR_UNSUPPORTED_CHARACTER);
    }
  }
  while (*data);

  handle_end_of_file(queue, &token, &data);

  return queue;
}

/**
 * @brief Read the data from the disk into memory and than feed
 *        the data into the lexer parser.
 */
syntax_queue_t *compile(const char *filepath)
{
  char data[MAXBUF];

  memset(data, 0, (MAXBUF * sizeof(char)));

  if (readfile(data, filepath) < 0)
  {
    const char errmsg[] = "could not read a file from the disk";
    die(errmsg, __FILE__, __func__);
  }

  return parse(data);
}
