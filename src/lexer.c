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
#include "ae.h"
#include "io.h"
#include "re.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUF    (1 << 20)

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
  /**
   * @brief Numerical Reserved Words.
   */
  ABSTRACT_RESERVED_WORD,
  BOOLEAN_RESERVED_WORD,
  BREAK_RESERVED_WORD,
  CASE_RESERVED_WORD,
  CLASS_RESERVED_WORD,
  CONST_RESERVED_WORD,
  DEFAULT_RESERVED_WORD,
  DOUBLE_RESERVED_WORD,
  EXPORT_RESERVED_WORD,
  FALSE_RESERVED_WORD,
  FLOAT_RESERVED_WORD,
  FOR_RESERVED_WORD,
  IF_RESERVED_WORD,
  IMMUTABLE_RESERVED_WORD,
  IMPORT_RESERVED_WORD,
  INTEGER_RESERVED_WORD,
  IS_RESERVED_WORD,
  MATRIX_RESERVED_WORD,
  NIL_RESERVED_WORD,
  OBJECT_RESERVED_WORD,
  PACKAGE_RESERVED_WORD,
  PRINT_RESERVED_WORD,
  PRIVATE_RESERVED_WORD,
  PROTECTED_RESERVED_WORD,
  PUBLIC_RESERVED_WORD,
  RETURN_RESERVED_WORD,
  SCALAR_RESERVED_WORD,
  SET_RESERVED_WORD,
  STATIC_RESERVED_WORD,
  STRING_RESERVED_WORD,
  SWITCH_RESERVED_WORD,
  TRUE_RESERVED_WORD,
  UNLESS_RESERVED_WORD,
  VECTOR_RESERVED_WORD,
  VOID_RESERVED_WORD,
  WHILE_RESERVED_WORD,
  YIELD_RESERVED_WORD,
};

static const char keywords[][9] = {
  "abstract",
  "boolean",
  "break",
  "case",
  "class",
  "const",
  "default",
  "double",
  "export",
  "false",
  "float",
  "for",
  "if",
  "immutable",
  "import",
  "int",
  "is",
  "matrix",
  "null",
  "object",
  "package",
  "print",
  "private",
  "protected",
  "public",
  "return",
  "scalar",
  "set",
  "static",
  "string",
  "switch",
  "true",
  "unless",
  "vector",
  "void",
  "while",
  "yield",
};

static const size_t keywords_size = sizeof(keywords) / sizeof(keywords[0]);

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

void print_token(token_t *token)
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

static void parse(const char *data)
{
  syntax_token_t token;

  do
  {
    if (ae_match(*data, AE_IS_WHITE))
    {
      do
      {
        switch (*data)
        {
          case '\t':
            token.type = TAB;
            token.i = data;
            break;

          case '\n':
            token.type = EOL;
            token.i = data;
            break;

          case ' ':
            token.type = SPACE;
            token.i = data;
            break;
        }

        data++;

        token.j = data;
        print_token(&token);
      }
      while (ae_match(*data, AE_IS_WHITE));
    }

    else if (ae_match(*data, AE_IS_DIGIT))
    {
      token.type = NUMBER;
      token.i = data;

      do
      {
        data++;

        if (*data == '.')
        {
          token.type = DECIMAL;
          data++;
        }
      }
      while (ae_match(*data, AE_IS_DIGIT));

      token.j = data;
      print_token(&token);
    }

    else if (ae_match(*data, AE_IS_ALNUM))
    {
      token.type = WORD;
      token.i = data;

      do
      {
        data++;
      }
      while (ae_match(*data, AE_IS_ALNUM));

      token.j = data;

      for (uint64_t i = 0; i < keywords_size; i++)
      {
        /**
         * NOTE: Use a data structure here to reduce the time complexity
         *       back to O(n), current O(n²).
         */
        if (strncmp(token.i, keywords[i], (token.j - token.i)) == 0)
        {
          switch (i)
          {
            case ABSTRACT_RESERVED_WORD:
              token.type = ABSTRACT;
              break;

            case BOOLEAN_RESERVED_WORD:
              token.type = BOOLEAN;
              break;

            case BREAK_RESERVED_WORD:
              token.type = BREAK;
              break;

            case CASE_RESERVED_WORD:
              token.type = CASE;
              break;

            case CLASS_RESERVED_WORD:
              token.type = CLASS;
              break;

            case CONST_RESERVED_WORD:
              token.type = CONST;
              break;

            case DEFAULT_RESERVED_WORD:
              token.type = DEFAULT;
              break;

            case DOUBLE_RESERVED_WORD:
              token.type = DOUBLE;
              break;

            case EXPORT_RESERVED_WORD:
              token.type = EXPORT;
              break;

            case FALSE_RESERVED_WORD:
              token.type = FALSE;
              break;

            case FLOAT_RESERVED_WORD:
              token.type = FLOAT;
              break;

            case FOR_RESERVED_WORD:
              token.type = FOR;
              break;

            case IF_RESERVED_WORD:
              token.type = IF;
              break;

            case IMMUTABLE_RESERVED_WORD:
              token.type = IMMUTABLE;
              break;

            case IMPORT_RESERVED_WORD:
              token.type = IMPORT;
              break;

            case INTEGER_RESERVED_WORD:
              token.type = INTEGER;
              break;

            case IS_RESERVED_WORD:
              token.type = IS;
              break;

            case MATRIX_RESERVED_WORD:
              token.type = MATRIX;
              break;

            case NIL_RESERVED_WORD:
              token.type = NIL;
              break;

            case OBJECT_RESERVED_WORD:
              token.type = OBJECT;
              break;

            case PACKAGE_RESERVED_WORD:
              token.type = PACKAGE;
              break;

            case PRINT_RESERVED_WORD:
              token.type = PRINT;
              break;

            case PRIVATE_RESERVED_WORD:
              token.type = PRIVATE;
              break;

            case PROTECTED_RESERVED_WORD:
              token.type = PROTECTED;
              break;

            case PUBLIC_RESERVED_WORD:
              token.type = PUBLIC;
              break;

            case RETURN_RESERVED_WORD:
              token.type = RETURN;
              break;

            case SCALAR_RESERVED_WORD:
              token.type = SCALAR;
              break;

            case SET_RESERVED_WORD:
              token.type = SET;
              break;

            case STATIC_RESERVED_WORD:
              token.type = STATIC;
              break;

            case STRING_RESERVED_WORD:
              token.type = STRING;
              break;

            case SWITCH_RESERVED_WORD:
              token.type = SWITCH;
              break;

            case TRUE_RESERVED_WORD:
              token.type = TRUE;
              break;

            case UNLESS_RESERVED_WORD:
              token.type = UNLESS;
              break;

            case VECTOR_RESERVED_WORD:
              token.type = VECTOR;
              break;

            case VOID_RESERVED_WORD:
              token.type = VOID;
              break;

            case WHILE_RESERVED_WORD:
              token.type = WHILE;
              break;

            case YIELD_RESERVED_WORD:
              token.type = YIELD;
              break;
          }
        }
      }

      print_token(&token);
    }

    else if (ae_match(*data, AE_IS_SYMBL))
    {
      if (*data == '\'')
      {
        token.type = TEXT;
        token.i = data;

        do
        {
          data++;
        }
        while (*data != 0 && *data != '\'');
      }
      else if (*data == '"')
      {
        token.type = TEXT;
        token.i = data;

        do
        {
          data++;
        }
        while (*data != 0 && *data != '"');
      }
      else if (*data == '@')
      {
        token.type = ANNOTATION;
        token.i = data;
      }
      else if (*data == '.')
      {
        token.type = DOT;
        token.i = data;
      }
      else if (*data == '<')
      {
        token.type = LEFT_CARET;
        token.i = data;
      }
      else if (*data == '>')
      {
        token.type = RIGHT_CARET;
        token.i = data;
      }
      else if (*data == '=')
      {
        if (*(data + 1) == '>')
        {
          data++;

          token.type = LAMBDA;
          token.i = data;
        }
        else if (*(data + 1) == '=')
        {
          data++;

          token.type = EQUALS;
          token.i = data;
        }
        else
        {
          token.type = EQUAL;
          token.i = data;
        }
      }
      else if (*data == ':')
      {
        token.type = COLON;
        token.i = data;
      }
      else if (*data == ',')
      {
        token.type = COMMA;
        token.i = data;
      }
      else if (*data == '(')
      {
        token.type = OPEN_PARENTHESIS;
        token.i = data;
      }
      else if (*data == ')')
      {
        token.type = CLOSE_PARENTHESIS;
        token.i = data;
      }
      else if (*data == '[')
      {
        token.type = OPEN_SQUARE_BRACKET;
        token.i = data;
      }
      else if (*data == ']')
      {
        token.type = CLOSE_SQUARE_BRACKET;
        token.i = data;
      }
      else if (*data == '{')
      {
        token.type = OPEN_CURLY_BRACKET;
        token.i = data;
      }
      else if (*data == '}')
      {
        token.type = CLOSE_CURLY_BRACKET;
        token.i = data;
      }
      else if (*data == '#')
      {
        token.type = REMAINDER;
        token.i = data;
      }
      else if (*data == '+')
      {
        if (*(data + 1) == '+')
        {
          data++;

          token.type = INCREMENT;
          token.i = data;
        }
        else
        {
          token.type = ADDITION;
          token.i = data;
        }
      }
      else if (*data == '-')
      {
        if (*(data + 1) == '-')
        {
          data++;

          token.type = DECREMENT;
          token.i = data;
        }
        else
        {
          token.type = SUBTRACTION;
          token.i = data;
        }
      }
      else if (*data == '/')
      {
        if (*(data + 1) == '/')
        {
          do
          {
            data++;
          }
          while (*data != 0 && *data != '\n');
        }
        else if (*(data + 1) == '*')
        {
          for (; *(data + 1); data++)
          {
            if (*data == '*' && *(data + 1) == '/')
            {
              data++;
              break;
            }
          }
        }
        else
        {
          token.type = DIVISION;
          token.i = data;
        }
      }
      else if (*data == '*')
      {
        if (*(data + 1) == '*')
        {
          data++;

          token.type = EXPONENTIAL;
          token.i = data;
        }
        else
        {
          token.type = STAR;
          token.i = data;
        }
      }
      else if (*data == '%')
      {
        token.type = MODULUS;
        token.i = data;
      }
      else if (*data == '^')
      {
        token.type = BITWISE_XOR;
        token.i = data;
      }
      else if (*data == '|')
      {
        if (*(data + 1) == '|')
        {
          data++;

          token.type = CONDITIONAL_OR;
          token.i = data;
        }
        else
        {
          token.type = BITWISE_OR;
          token.i = data;
        }
      }
      else if (*data == '&')
      {
        if (*(data + 1) == '&')
        {
          data++;

          token.type = CONDITIONAL_AND;
          token.i = data;
        }
        else
        {
          token.type = BITWISE_AND;
          token.i = data;
        }
      }
      else if (*data == '~')
      {
        token.type = BITWISE_TERNARY;
        token.i = data;
      }
      else if (*data == ';')
      {
        token.type = EOE;
        token.i = data;
      }
      else
      {
        token.type = SYMBOL;
        token.i = data;
      }

      data++;

      token.j = data;
      print_token(&token);
    }

    else
    {
      printf("unsupported character: %c\n", *data);
      const char errmsg[] = "The compiler encountered an unsupported character";
      die(errmsg, __FILE__, __func__);
    }
  }
  while (*data);

  token.type = LEXER_EOF;
  token.i = data;
  token.j = data;

  print_token(&token);
}

/**
 * @brief Read the data from the disk into memory and than feed
 *        the data into the lexer parser.
 */
void compile(const char *filepath)
{
  char data[MAXBUF];

  memset(data, 0, (MAXBUF * sizeof(char)));

  if (readfile(data, filepath) < 0)
  {
    const char errmsg[] = "could not read a file from the disk";
    die(errmsg, __FILE__, __func__);
  }

  parse(data);
}
