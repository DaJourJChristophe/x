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
#include "facts.h"
#include "io.h"
#include "lexer.h"
#include "syntax-queue.h"
#include "token.h"

#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUF      (1 << 20)
#define MAX_QUEUE   1024

#define LEXER_HASH_INIT   5381

/**
 * @brief Hash a string, based on the djb2 algorithm, return the hash
 *        via a pointer parameter. This algorithm (k=33) was first
 *        reported by Dan Bernstein many years ago in comp.lang.c.
 *        another version of this algorithm (now favored by bernstein)
 *        uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of
 *        number 33 (why it works better than many other constants,
 *        prime or not) has never been adequately explained.
 */
static void hash(uint64_t *hash, const char **s, char *buffer, size_t *size)
{
  for (; ae_match(**s, ae_is_alpha); *s += 1, buffer++, *size += 1)
  {
    *hash = (*hash << 5) + *hash + **s;

    *buffer = **s;
  }
}

#define INLINE_VOID_T inline void always_inline

// static const int whitespace_syntax_tokens[128] = {
//   0, 0, 0, 0, 0, 0, 0, 0, 0,
//   TAB,
//   EOL,
//   0, 0, 0, 0, 0, 0, 0, 0, 0,
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//   SPACE,
//   0, 0, 0, 0, 0, 0, 0,
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//   0, 0, 0, 0, 0, 0, 0, 0,
// };

// static INLINE_VOID_T handle_whitespace(syntax_queue_t *queue, syntax_token_t *token, const char **data)
// {
//   int index;

//   do
//   {
//     index = **data;

//     token->type = whitespace_syntax_tokens[index];
//     token->data = NULL;
//     token->size = 0;

//     *data += 1;

//     if (syntax_queue_write(queue, token) == false)
//     {
//       die(X_ERROR_SYNTAX_QUEUE_WRITE, __FILE__, __func__);
//     }
//   }
//   while (ae_match(**data, ae_is_space));
// }

static void parse_integer(const char **s, int *y)
{
  int r = 0;
  int n = 1;
  int x;

  /**
   * NOTE: Technically, this does not apply to the lexer as the lexer
   *       handles whitespace.
   */
  for (; **s; *s += 1)
  {
    if (**s != 0x20)
    {
      break;
    }
  }

  if (**s == 0)
  {
    *y = n * r;
    return;
  }

  if (**s == 0x2d)
  {
    n = (-1);
    *s += 1;
  }
  else if (**s == 0x2b)
  {
    n = 1;
    *s += 1;
  }

  for (; **s; *s += 1)
  {
    if (ae_match(**s, ae_is_digit) == false)
    {
      goto done;
    }

    x = **s - 0x30;

    if ((r > (INT_MAX / 10) || (r == (INT_MAX / 10) && x >  7)) ||
        (r < (INT_MIN / 10) || (r == (INT_MIN / 10) && x < -8)))
    {
      *y = (n < 1) ? INT_MIN : INT_MAX;
      return;
    }

    r = 10 * r + x;
  }

done:
  *y = n * r;
  // return (n * r);
}

static INLINE_VOID_T handle_number(syntax_queue_t *queue, syntax_token_t *token, const char **data)
{
  token->type = INT_TOK;
  token->size = sizeof(int);

  token->data = __malloc(sizeof(int));
  parse_integer(data, token->data);

  // do
  // {
  //   *data += 1;

  //   if (**data == '.')
  //   {
  //     token->type = DECIMAL;
  //     *data += 1;
  //   }
  // }
  // while (ae_match(**data, ae_is_digit));

  if (syntax_queue_write(queue, token) == false)
  {
    die(X_ERROR_SYNTAX_QUEUE_WRITE, __FILE__, __func__);
  }
}

static INLINE_VOID_T handle_word(syntax_queue_t *queue, syntax_token_t *token, const char **data)
{
  uint64_t _hash = LEXER_HASH_INIT;

  token->type = WORD_TOK;
  token->size = 0;

  token->data = __malloc(64 * sizeof(char));
  memset(token->data, 0, (64 * sizeof(char)));
  hash(&_hash, data, token->data, &token->size);

  // switch (_hash)
  // {
  //   case ABSTRACT_RESERVED_WORD:
  //     token->type = ABSTRACT;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case BOOLEAN_RESERVED_WORD:
  //     token->type = BOOLEAN;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case BREAK_RESERVED_WORD:
  //     token->type = BREAK;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case CASE_RESERVED_WORD:
  //     token->type = CASE;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case CLASS_RESERVED_WORD:
  //     token->type = CLASS;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case CONST_RESERVED_WORD:
  //     token->type = CONST;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case DEFAULT_RESERVED_WORD:
  //     token->type = DEFAULT;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case DOUBLE_RESERVED_WORD:
  //     token->type = DOUBLE;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case EXPORT_RESERVED_WORD:
  //     token->type = EXPORT;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case FALSE_RESERVED_WORD:
  //     token->type = FALSE;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->data = malloc(sizeof(int));
  //     *(int *)token->data = (int)0;
  //     token->size = sizeof(int);
  //     break;

  //   case FLOAT_RESERVED_WORD:
  //     token->type = FLOAT;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case FOR_RESERVED_WORD:
  //     token->type = FOR;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case IF_RESERVED_WORD:
  //     token->type = IF;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case IMMUTABLE_RESERVED_WORD:
  //     token->type = IMMUTABLE;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case IMPORT_RESERVED_WORD:
  //     token->type = IMPORT;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case INTEGER_RESERVED_WORD:
  //     token->type = INTEGER;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case IS_RESERVED_WORD:
  //     token->type = IS;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case MATRIX_RESERVED_WORD:
  //     token->type = MATRIX;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case NIL_RESERVED_WORD:
  //     token->type = NIL;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case OBJECT_RESERVED_WORD:
  //     token->type = OBJECT;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case PACKAGE_RESERVED_WORD:
  //     token->type = PACKAGE;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case PRINT_RESERVED_WORD:
  //     token->type = PRINT;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case PRIVATE_RESERVED_WORD:
  //     token->type = PRIVATE;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case PROTECTED_RESERVED_WORD:
  //     token->type = PROTECTED;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case PUBLIC_RESERVED_WORD:
  //     token->type = PUBLIC;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case RETURN_RESERVED_WORD:
  //     token->type = RETURN;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case SCALAR_RESERVED_WORD:
  //     token->type = SCALAR;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case SET_RESERVED_WORD:
  //     token->type = SET;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case STATIC_RESERVED_WORD:
  //     token->type = STATIC;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case STRING_RESERVED_WORD:
  //     token->type = STRING;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case SWITCH_RESERVED_WORD:
  //     token->type = SWITCH;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case TRUE_RESERVED_WORD:
  //     token->type = TRUE;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->data = malloc(sizeof(int));
  //     *(int *)token->data = (int)1;
  //     token->size = sizeof(int);
  //     break;

  //   case UNLESS_RESERVED_WORD:
  //     token->type = UNLESS;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case VECTOR_RESERVED_WORD:
  //     token->type = VECTOR;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case VOID_RESERVED_WORD:
  //     token->type = VOID;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case WHILE_RESERVED_WORD:
  //     token->type = WHILE;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;

  //   case YIELD_RESERVED_WORD:
  //     token->type = YIELD;
  //     __free(token->data);
  //     token->data = NULL;
  //     token->size = 0;
  //     break;
  // }

  if (syntax_queue_write(queue, token) == false)
  {
    die(X_ERROR_SYNTAX_QUEUE_WRITE, __FILE__, __func__);
  }
}

// typedef void (*symbol_handler_t)(syntax_token_t *, const char **);

// static void handle_symbol_single_quote(syntax_token_t *token, const char **data)
// {
//   token->type = TEXT;
//   token->size = 0;

//   token->data = __malloc(64 * sizeof(char));
//   memset(token->data, 0, (64 * sizeof(char)));

//   char *p = token->data;

//   do
//   {
//     *data += 1;
//     *p = **data;
//     *p += 1;
//     token->size++;
//   }
//   while (**data != 0 && **data != '\'');
// }

// static void handle_symbol_double_quote(syntax_token_t *token, const char **data)
// {
//   token->type = TEXT;
//   token->size = 0;

//   token->data = __malloc(64 * sizeof(char));
//   memset(token->data, 0, (64 * sizeof(char)));

//   char *p = token->data;

//   do
//   {
//     *data += 1;
//     *p = **data;
//     *p += 1;
//     token->size++;
//   }
//   while (**data != 0 && **data != '"');
// }

// static void handle_symbol_left_caret(syntax_token_t *token, const char **data)
// {
//   if (*(*data + 1) == '<')
//   {
//     *data += 1;

//     token->type = BITWISE_SHIFT_LEFT;
//     token->data = NULL;
//     token->size = 0;
//   }
//   else
//   {
//     token->type = LEFT_CARET;
//     token->data = NULL;
//     token->size = 0;
//   }
// }

// static void handle_symbol_right_caret(syntax_token_t *token, const char **data)
// {
//   if (*(*data + 1) == '>')
//   {
//     *data += 1;

//     token->type = BITWISE_SHIFT_RIGHT;
//     token->data = NULL;
//     token->size = 0;
//   }
//   else
//   {
//     token->type = RIGHT_CARET;
//     token->data = NULL;
//     token->size = 0;
//   }
// }

// static void handle_symbol_equal_sign(syntax_token_t *token, const char **data)
// {
//   if (*(*data + 1) == '>')
//   {
//     *data += 1;

//     token->type = LAMBDA;
//     token->data = NULL;
//     token->size = 0;
//   }
//   else if (*(*data + 1) == '=')
//   {
//     *data += 1;

//     token->type = EQUALS;
//     token->data = NULL;
//     token->size = 0;
//   }
//   else
//   {
//     token->type = EQUAL;
//     token->data = NULL;
//     token->size = 0;
//   }
// }

// static void handle_symbol_plus_sign(syntax_token_t *token, const char **data)
// {
//   if (*(*data + 1) == '+')
//   {
//     *data += 1;

//     token->type = INCREMENT;
//     token->data = NULL;
//     token->size = 0;
//   }
//   else
//   {
//     token->type = ADDITION;
//     token->data = NULL;
//     token->size = 0;
//   }
// }

// static void handle_symbol_minus_sign(syntax_token_t *token, const char **data)
// {
//   if (*(*data + 1) == '-')
//   {
//     *data += 1;

//     token->type = DECREMENT;
//     token->data = NULL;
//     token->size = 0;
//   }
//   else
//   {
//     token->type = SUBTRACTION;
//     token->data = NULL;
//     token->size = 0;
//   }
// }

// static void handle_symbol_forward_slash(syntax_token_t *token, const char **data)
// {
//   if (*(*data + 1) == '/')
//   {
//     do
//     {
//       *data += 1;
//     }
//     while (**data != 0 && **data != '\n');
//   }
//   else if (*(*data + 1) == '*')
//   {
//     for (; *(*data + 1); *data += 1)
//     {
//       if (**data == '*' && *(*data + 1) == '/')
//       {
//         *data += 1;
//         break;
//       }
//     }
//   }
//   else
//   {
//     token->type = DIVISION;
//     token->data = NULL;
//     token->size = 0;
//   }
// }

// static void handle_symbol_star(syntax_token_t *token, const char **data)
// {
//   if (*(*data + 1) == '*')
//   {
//     *data += 1;

//     token->type = EXPONENTIAL;
//     token->data = NULL;
//     token->size = 0;
//   }
//   else
//   {
//     token->type = STAR;
//     token->data = NULL;
//     token->size = 0;
//   }
// }

// static void handle_symbol_vertical_bar(syntax_token_t *token, const char **data)
// {
//   if (*(*data + 1) == '|')
//   {
//     *data += 1;

//     token->type = CONDITIONAL_OR;
//     token->data = NULL;
//     token->size = 0;
//   }
//   else
//   {
//     token->type = BITWISE_OR;
//     token->data = NULL;
//     token->size = 0;
//   }
// }

// static void handle_symbol_ampersand(syntax_token_t *token, const char **data)
// {
//   if (*(*data + 1) == '&')
//   {
//     *data += 1;

//     token->type = CONDITIONAL_AND;
//     token->data = NULL;
//     token->size = 0;
//   }
//   else
//   {
//     token->type = BITWISE_AND;
//     token->data = NULL;
//     token->size = 0;
//   }
// }

// /* Define the maximum depth of a character combination. */
// #define MAX_COMB_DEPTH   ((int)3)

// /* Define the maximum possible character value range. */
// #define MAX_CHAR_RNG     ((int)128)

// /**
//  * @brief
//  */
// static const int symbl_table[MAX_COMB_DEPTH][MAX_CHAR_RNG] = {
//   {
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, BITWISE_AND, 0, 0, 0, 0, 0, 0, 0, DOT, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, BITWISE_OR, 0, 0, 0,
//   },
//   {
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, CONDITIONAL_AND, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CONDITIONAL_OR, 0, 0, 0,
//   },
//   {
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//   },
// };

// /**
//  * @brief Select a token type from the character lookup table.
//  */
// static int gettok(const char *data)
// {
//   const int n = MAX_COMB_DEPTH;
//   int selection;
//   int i = 0;

//   do
//   {
//     selection = symbl_table[i++][(int)*(data++)];
//   }
//   while (*data && i < n && selection);

//   return selection;
// }

// static const symbol_handler_t multiple_symbol_handlers[128] = {
//   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
//   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
//   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
//   NULL, NULL, NULL, NULL,
//   &handle_symbol_double_quote,
//   NULL, NULL, NULL,
//   &handle_symbol_ampersand,
//   &handle_symbol_single_quote,
//   NULL, NULL,
//   &handle_symbol_star,
//   &handle_symbol_plus_sign,
//   NULL,
//   &handle_symbol_minus_sign,
//   NULL,
//   &handle_symbol_forward_slash,
//   NULL, NULL,
//   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
//   &handle_symbol_left_caret,
//   &handle_symbol_equal_sign,
//   &handle_symbol_right_caret,
//   NULL, NULL, NULL, NULL, NULL, NULL, NULL,
//   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
//   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
//   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
//   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
//   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
//   NULL, NULL, NULL, NULL,
//   &handle_symbol_vertical_bar,
//   NULL, NULL, NULL,
// };

// static const int symbol_syntax_tokens[128] = {
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//   REMAINDER,
//   0,
//   MODULUS,
//   0, 0,
//   OPEN_PARENTHESIS,
//   CLOSE_PARENTHESIS,
//   0, 0,
//   COMMA,
//   0,
//   DOT,
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//   COLON,
//   EOE,
//   0,
//   0,
//   0,
//   0,
//   ANNOTATION,
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//   OPEN_SQUARE_BRACKET,
//   0,
//   CLOSE_SQUARE_BRACKET,
//   BITWISE_XOR,
//   0, 0, 0, 0, 0,
//   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//   0, 0, 0,
//   OPEN_CURLY_BRACKET,
//   0,
//   CLOSE_CURLY_BRACKET,
//   BITWISE_TERNARY,
//   0,
// };

// static INLINE_VOID_T handle_symbol(syntax_queue_t *queue, syntax_token_t *token, const char **data)
// {
//   int selection;

//   selection = gettok(data);

//   if (selection == 0)
//   {
//     throw(X_ERROR_UNSUPPORTED_CHARACTER);
//   }

//   token->type = selection;
//   token->data = NULL;
//   token->size = 0;

//   // int index = **data;
//   // symbol_handler_t multiple_handler = NULL;

//   // multiple_handler = multiple_symbol_handlers[index];

//   // if (multiple_handler != NULL)
//   // {
//   //   multiple_handler(token, data);
//   // }
//   // else
//   // {
//   //   const int probable_token = symbol_syntax_tokens[index];

//   //   if (probable_token != 0)
//   //   {
//   //     token->type = probable_token;
//   //     token->data = NULL;
//   //     token->size = 0;
//   //   }
//   //   else
//   //   {
//   //     token->type = SYMBOL;
//   //     token->data = NULL;
//   //     token->size = 0;
//   //   }
//   // }

//   // *data += 1;

//   if (syntax_queue_write(queue, token) == false)
//   {
//     die(X_ERROR_SYNTAX_QUEUE_WRITE, __FILE__, __func__);
//   }
// }

static INLINE_VOID_T handle_end_of_file(syntax_queue_t *queue, syntax_token_t *token)
{
  token->type = EOF_TOK;
  token->data = NULL;
  token->size = 0;

  if (syntax_queue_write(queue, token) == false)
  {
    die(X_ERROR_SYNTAX_QUEUE_WRITE, __FILE__, __func__);
  }
}

#define SYNTAX_TOKEN_NODE_CHILD_MAX    ((int)128)

/**
 * @brief Define the syntax token node data structure.
 */
struct syntax_token_node
{
  syntax_token_t tok;
  bool isWordEnd;

  /* An array of syntax token nodes representing the current node's children. */
  struct syntax_token_node **children;
};

/**
 * @brief Define a namespace for the syntax token node structure.
 */
typedef struct syntax_token_node syntax_token_node_t;

/**
 * @brief
 */
syntax_token_node_t *syntax_token_node_new(const syntax_token_t *tok)
{
  syntax_token_node_t *node = NULL;
  node = __malloc(sizeof(syntax_token_node_t));
  node->children = NULL;
  memcpy(&node->tok, tok, sizeof(syntax_token_t));
  return node;
}

/**
 * @brief
 */
void syntax_token_node_destroy(syntax_token_node_t *node)
{
  if (node != NULL && node->children != NULL)
  {
    syntax_token_node_t **children = node->children;

    for (uint64_t i = 0; i < SYNTAX_TOKEN_NODE_CHILD_MAX; i++)
    {
      if (children[i] != NULL)
      {
        syntax_token_node_destroy(children[i]);
      }
    }

    __free(node->children);
  }

  __free(node);
}

/**
 * @brief Define the syntax token node data structure.
 */
struct syntax_token_trie
{
  syntax_token_node_t *root;
};

/**
 * @brief Define a namespace for the syntax token node structure.
 */
typedef struct syntax_token_trie syntax_token_trie_t;

/**
 * @brief
 */
syntax_token_trie_t *syntax_token_trie_new(void)
{
  syntax_token_trie_t *trie = NULL;
  trie = __malloc(sizeof(syntax_token_trie_t));
  trie->root = syntax_token_node_new(&(syntax_token_t){
    .type=0,
    .data=NULL,
    .size=0,
  });
  return trie;
}

/**
 * @brief
 */
void syntax_token_trie_destroy(syntax_token_trie_t *trie)
{
  if (trie != NULL)
  {
    syntax_token_node_destroy(trie->root);
  }
  __free(trie);
}

/**
 * @brief
 */
bool syntax_token_trie_search(syntax_token_trie_t *trie, syntax_token_t *rettok, const char **key)
{
  syntax_token_node_t *node = NULL;

  bool result = false;

  int index;
  int jndex;

  node = trie->root;

  memset(rettok, 0, sizeof(syntax_token_t));
  memcpy(rettok, &(syntax_token_t){.type=BAD_TOK,.data=NULL,.size=0,}, sizeof(syntax_token_t));

  for (; **key; (*key)++)
  {
    index = (int)**key;
    jndex = (int)*(*key + 1);

    if (node->children == NULL || node->children[index] == NULL)
    {
      goto done;
    }

    node = node->children[index];

    if (node->children != NULL && node->children[jndex] != NULL)
    {
      continue;
    }

    *key += 1;

    if (node->isWordEnd)
    {
      goto setret;
    }
    else
    {
      goto done;
    }
  }
setret:
  result = true;
  memset(rettok, 0, sizeof(syntax_token_t));
  memcpy(rettok, &node->tok, sizeof(syntax_token_t));
done:
  return result;
}

/**
 * @brief
 */
void syntax_token_trie_insert(syntax_token_trie_t *trie, const char *key, const syntax_token_t *tok)
{
  syntax_token_node_t *node = NULL;
  int index;

  node = trie->root;

  for (; *key; key++)
  {
    index = (int)*key;

    if (node->children == NULL)
    {
      node->children = __calloc(SYNTAX_TOKEN_NODE_CHILD_MAX, sizeof(syntax_token_node_t *));
    }

    if (node->children[index] == NULL)
    {
      node->children[index] = syntax_token_node_new(tok);
    }

    node = node->children[index];
  }

  node->isWordEnd = true;
}

/**
 * @brief
 */
syntax_token_trie_t *global_token_diction;

/**
 * @brief
 */
struct syntax_token_insert
{
  const char *str;
  syntax_token_t tok;
};

/**
 * @brief
 */
typedef struct syntax_token_insert syntax_token_insert_t;

/**
 * @brief
 */
static const syntax_token_insert_t insertions[] = {
  {
    .str="\0",
    .tok=(syntax_token_t){
      .type=BAD_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="\t",
    .tok=(syntax_token_t){
      .type=TAB_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="\n",
    .tok=(syntax_token_t){
      .type=EOL_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str=" ",
    .tok=(syntax_token_t){
      .type=SPACE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="\"",
    .tok=(syntax_token_t){
      .type=DOUBLE_QUOTATION_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="#",
    .tok=(syntax_token_t){
      .type=REMAINDER_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="\%",
    .tok=(syntax_token_t){
      .type=MODULUS_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="&",
    .tok=(syntax_token_t){
      .type=BITWISE_AND_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="'",
    .tok=(syntax_token_t){
      .type=SINGLE_QUOTATION_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="(",
    .tok=(syntax_token_t){
      .type=OPEN_PARENTHESIS_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str=")",
    .tok=(syntax_token_t){
      .type=CLOSE_PARENTHESIS_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="*",
    .tok=(syntax_token_t){
      .type=STAR_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="+",
    .tok=(syntax_token_t){
      .type=PLUS_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str=",",
    .tok=(syntax_token_t){
      .type=COMMA_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="-",
    .tok=(syntax_token_t){
      .type=HYPHEN_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str=".",
    .tok=(syntax_token_t){
      .type=DOT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="/",
    .tok=(syntax_token_t){
      .type=SLASH_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str=":",
    .tok=(syntax_token_t){
      .type=COLON_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str=";",
    .tok=(syntax_token_t){
      .type=EOE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="<",
    .tok=(syntax_token_t){
      .type=LEFT_CARET_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="=",
    .tok=(syntax_token_t){
      .type=EQUAL_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str=">",
    .tok=(syntax_token_t){
      .type=RIGHT_CARET_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="?",
    .tok=(syntax_token_t){
      .type=QUESTION_MARK_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="@",
    .tok=(syntax_token_t){
      .type=ANNOTATION_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="[",
    .tok=(syntax_token_t){
      .type=OPEN_SQUARE_BRACKET_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="\\",
    .tok=(syntax_token_t){
      .type=BACKSLASH_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="]",
    .tok=(syntax_token_t){
      .type=CLOSE_SQUARE_BRACKET_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="^",
    .tok=(syntax_token_t){
      .type=BITWISE_XOR_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="_",
    .tok=(syntax_token_t){
      .type=UNDERSCORE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="`",
    .tok=(syntax_token_t){
      .type=APOSTROPHE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="{",
    .tok=(syntax_token_t){
      .type=OPEN_CURLY_BRACKET_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="|",
    .tok=(syntax_token_t){
      .type=BITWISE_OR_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="}",
    .tok=(syntax_token_t){
      .type=CLOSE_CURLY_BRACKET_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="~",
    .tok=(syntax_token_t){
      .type=BITWISE_TERNARY_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="**",
    .tok=(syntax_token_t){
      .type=EXPONENTIAL_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="&&",
    .tok=(syntax_token_t){
      .type=COND_AND_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="||",
    .tok=(syntax_token_t){
      .type=COND_OR_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="<<",
    .tok=(syntax_token_t){
      .type=BITWISE_SHFT_LEFT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str=">>",
    .tok=(syntax_token_t){
      .type=BITWISE_SHFT_RIGHT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="--",
    .tok=(syntax_token_t){
      .type=DECREMENT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="++",
    .tok=(syntax_token_t){
      .type=INCREMENT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="==",
    .tok=(syntax_token_t){
      .type=COND_EQUALS_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="abstract",
    .tok=(syntax_token_t){
      .type=ABSTRACT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="boolean",
    .tok=(syntax_token_t){
      .type=BOOLEAN_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="break",
    .tok=(syntax_token_t){
      .type=BREAK_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="case",
    .tok=(syntax_token_t){
      .type=CASE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="class",
    .tok=(syntax_token_t){
      .type=CLASS_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="const",
    .tok=(syntax_token_t){
      .type=CONST_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="default",
    .tok=(syntax_token_t){
      .type=DEFAULT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="double",
    .tok=(syntax_token_t){
      .type=DOUBLE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="export",
    .tok=(syntax_token_t){
      .type=EXPORT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="false",
    .tok=(syntax_token_t){
      .type=FALSE_TOK,
      .data=&(int){0},
      .size=sizeof(int),
    },
  }, {
    .str="float",
    .tok=(syntax_token_t){
      .type=FLOAT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="for",
    .tok=(syntax_token_t){
      .type=FOR_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="if",
    .tok=(syntax_token_t){
      .type=IF_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="immutable",
    .tok=(syntax_token_t){
      .type=IMMUTABLE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="import",
    .tok=(syntax_token_t){
      .type=IMPORT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="int",
    .tok=(syntax_token_t){
      .type=INTEGER_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="is",
    .tok=(syntax_token_t){
      .type=IS_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="matrix",
    .tok=(syntax_token_t){
      .type=MATRIX_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="null",
    .tok=(syntax_token_t){
      .type=NIL_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="object",
    .tok=(syntax_token_t){
      .type=OBJECT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="package",
    .tok=(syntax_token_t){
      .type=PACKAGE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="print",
    .tok=(syntax_token_t){
      .type=PRINT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="private",
    .tok=(syntax_token_t){
      .type=PRIVATE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="protected",
    .tok=(syntax_token_t){
      .type=PROTECTED_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="public",
    .tok=(syntax_token_t){
      .type=PUBLIC_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="return",
    .tok=(syntax_token_t){
      .type=RETURN_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="scalar",
    .tok=(syntax_token_t){
      .type=SCALAR_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="set",
    .tok=(syntax_token_t){
      .type=SET_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="static",
    .tok=(syntax_token_t){
      .type=STATIC_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="string",
    .tok=(syntax_token_t){
      .type=STRING_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="switch",
    .tok=(syntax_token_t){
      .type=SWITCH_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="true",
    .tok=(syntax_token_t){
      .type=TRUE_TOK,
      .data=&(int){1},
      .size=sizeof(int),
    },
  }, {
    .str="unless",
    .tok=(syntax_token_t){
      .type=UNLESS_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="vector",
    .tok=(syntax_token_t){
      .type=VECTOR_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="void",
    .tok=(syntax_token_t){
      .type=VOID_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="while",
    .tok=(syntax_token_t){
      .type=WHILE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="yield",
    .tok=(syntax_token_t){
      .type=YIELD_TOK,
      .data=NULL,
      .size=0,
    },
  },
};

/**
 * @brief
 */
static const size_t count = sizeof(insertions) / sizeof(insertions[0]);

/**
 * @brief
 */
static syntax_queue_t *parse(syntax_token_trie_t *trie, const char *data)
{
  const char **ptr = &data;
  syntax_queue_t *queue = NULL;
  syntax_token_t rettok, *retptr = &rettok;

  queue = syntax_queue_new(MAX_QUEUE);

  do
  {
    const char *start = *ptr;

    /**
     * NOTE: If the literal search does not work attempt a pattern search.
     */
    if (syntax_token_trie_search(trie, retptr, ptr) == false)
    {
      *ptr = start;

      if (ae_match(**ptr, ae_is_digit))
      {
        handle_number(queue, retptr, ptr);
      }
      else if (ae_match(**ptr, ae_is_alnum))
      {
        handle_word(queue, retptr, ptr);
      }
      else
      {
        throw(X_ERROR_UNSUPPORTED_CHARACTER);
      }
    }

    if (syntax_queue_write(queue, retptr) == false)
    {
      die(X_ERROR_SYNTAX_QUEUE_WRITE, __FILE__, __func__);
    }
  }
  while (**ptr);
  handle_end_of_file(queue, retptr);
  return queue;
}

// static syntax_queue_t *parse(const char *data)
// {
//   syntax_queue_t *queue = NULL;
//   syntax_token_t token;

//   queue = syntax_queue_new(MAX_QUEUE);

//   do
//   {
//     if (ae_match(*data, ae_is_space))
//     {
//       handle_whitespace(queue, &token, &data);
//     }

//     else if (ae_match(*data, ae_is_digit))
//     {
//       handle_number(queue, &token, &data);
//     }

//     else if (ae_match(*data, ae_is_alnum))
//     {
//       handle_word(queue, &token, &data);
//     }

//     else if (ae_match(*data, ae_is_symbl))
//     {
//       handle_symbol(queue, &token, &data);
//     }

//     else
//     {
//       throw(X_ERROR_UNSUPPORTED_CHARACTER);
//     }
//   }
//   while (*data);

//   handle_end_of_file(queue, &token);

//   return queue;
// }

static int init(void)
{
  /* NOTE: Allocates the global token dictionary to the heap. */
  global_token_diction = syntax_token_trie_new();

  /* NOTE: Adds all tokens to the global token dictionary. */
  for (uint64_t i = 0; i < count; i++)
  {
    syntax_token_trie_insert(global_token_diction,   insertions[i].str,
                                                    &insertions[i].tok);
  }

  return 0;
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

  init();

  return parse(global_token_diction, data);
}

syntax_queue_t *compile_line(const char *data)
{
  init();

  return parse(global_token_diction, data);
}
