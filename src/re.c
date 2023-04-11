/**
 * @file re.c
 * @author Da'Jour J. Christophe (dajour.christophe@gmail.com)
 * @brief Mini regex-module inspired by Rob Pike's regex code described in:
 *        http://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html
 * @version 0.1
 * @date 2023-04-08
 *
 * @copyright Copyright (c) 2023 Da'Jour J. Christophe. All rights reserved.
 *
 * Supports:
 * ---------
 *   '.'        Dot, matches any character
 *   '^'        Start anchor, matches beginning of string
 *   '$'        End anchor, matches end of string
 *   '*'        Asterisk, match zero or more (greedy)
 *   '+'        Plus, match one or more (greedy)
 *   '?'        Question, match zero or one (non-greedy)
 *   '[abc]'    Character class, match if one of {'a', 'b', 'c'}
 *   '[^abc]'   Inverted class, match if NOT one of {'a', 'b', 'c'} -- NOTE: feature is currently broken!
 *   '[a-zA-Z]' Character ranges, the character set of the ranges { a-z | A-Z }
 *   '\s'       Whitespace, \t \f \r \n \v and spaces
 *   '\S'       Non-whitespace
 *   '\w'       Alphanumeric, [a-zA-Z0-9_]
 *   '\W'       Non-alphanumeric
 *   '\d'       Digits, [0-9]
 *   '\D'       Non-digits
 */
#include "ae.h"
#include "re.h"

/**
 * @brief Max number of regex symbols in expression.
 */
#define MAX_REGEXP_OBJECTS   30

/**
 * @brief Max length of character-class buffer in.
 */
#define MAX_CHAR_CLASS_LEN   40

enum
{
  UNUSED,
  DOT,
  BEGIN,
  END,
  QUESTIONMARK,
  STAR,
  PLUS,
  CHAR,
  CHAR_CLASS,
  INV_CHAR_CLASS,
  DIGIT,
  NOT_DIGIT,
  ALPHA,
  NOT_ALPHA,
  WHITESPACE,
  NOT_WHITESPACE,
  /* BRANCH */
};

/**
 * @property type - CHAR, STAR, etc.
 * @property cch - the character itself.
 * @property ccl - OR  a pointer to characters in class.
 */
typedef struct regex_t
{
  unsigned char type;

  union
  {
    unsigned char  ch;
    unsigned char* ccl;
  } u;
} regex_t;

static int match_pattern(regex_t *pattern, const char *text, int *matchlength);
static int match_class(const char c, const char* str);
static int match_star(regex_t p, regex_t *pattern, const char *text, int *matchlength);
static int match_plus(regex_t p, regex_t *pattern, const char *text, int *matchlength);
static int match_one(regex_t p, const char c);
static int match_digit(const char c);
static int match_alpha(const char c);
static int match_space(const char c);
static int match_meta(const char c, const char *str);
static int match_range(const char c, const char *str);
static int match_dot(const char c);
static int is_meta(const char c);

/**
 * @brief Call `re_compile` and then call `re_match_compiled`.
 */
int re_match(const char *pattern, const char *text, int *length)
{
  return re_match_compiled(re_compile(pattern), text, length);
}

/**
 * @brief Match a pattern from beginning if `BEGIN` is specified or
 *        else iterate over text and continuously match a pattern.
 */
int re_match_compiled(re_t pattern, const char *text, int *length)
{
  *length = 0;

  if (pattern == 0)
  {
    return (-1);
  }

  if (pattern[0].type == BEGIN)
  {
    return match_pattern(&pattern[1], text, length) ? 0 : (-1);
  }

  int idx = (-1);

  do
  {
    idx++;

    if (match_pattern(pattern, text, length))
    {
      return text[0] == '\0' ? (-1) : idx;
    }
  }
  while (*text++ != '\0');

  return (-1);
}

/**
 * @brief Compile a pattern into a regular expression object.
 *
 * The sizes of the two static arrays below substantiates the static RAM usage of this module.
 * MAX_REGEXP_OBJECTS is the max number of symbols in the expression.
 * MAX_CHAR_CLASS_LEN determines the size of buffer for chars in all char-classes in the expression.
 */
re_t re_compile(const char *pattern)
{
  static regex_t re_compiled[MAX_REGEXP_OBJECTS];
  static unsigned char ccl_buf[MAX_CHAR_CLASS_LEN];
  int ccl_bufidx = 1;

  char c;     /* current char in pattern   */
  int i = 0;  /* index into pattern        */
  int j = 0;  /* index into re_compiled    */

  while (pattern[i] != '\0' && (j + 1) < MAX_REGEXP_OBJECTS)
  {
    c = pattern[i];

    switch (c)
    {
      case '^': {    re_compiled[j].type = BEGIN;           } break;
      case '$': {    re_compiled[j].type = END;             } break;
      case '.': {    re_compiled[j].type = DOT;             } break;
      case '*': {    re_compiled[j].type = STAR;            } break;
      case '+': {    re_compiled[j].type = PLUS;            } break;
      case '?': {    re_compiled[j].type = QUESTIONMARK;    } break;
/*    case '|': {    re_compiled[j].type = BRANCH;          } break; <-- not working properly */

      /*
       * Escaped character-classes (\s \w ...):
       */

      case '\\':
      {
        if (pattern[i+1] != '\0')
        {
          i += 1;                  /* Skip the escape-char '\\' */
                                   /* ... and check the next    */

          switch (pattern[i])
          {
            case 'd': { re_compiled[j].type = DIGIT;          } break;
            case 'D': { re_compiled[j].type = NOT_DIGIT;      } break;
            case 'w': { re_compiled[j].type = ALPHA;          } break;
            case 'W': { re_compiled[j].type = NOT_ALPHA;      } break;
            case 's': { re_compiled[j].type = WHITESPACE;     } break;
            case 'S': { re_compiled[j].type = NOT_WHITESPACE; } break;

            default:    /* Escaped character, e.g. '.' or '$' */
            {
              re_compiled[j].type = CHAR;
              re_compiled[j].u.ch = pattern[i];
            } break;
          }
        }

        /* '\\' as last char in pattern -> invalid regular expression. */
/*
        else
        {
          re_compiled[j].type = CHAR;
          re_compiled[j].ch = pattern[i];
        }
*/
      } break;

      /* Character class: */
      case '[':
      {
        /* Remember where the char-buffer starts. */
        int buf_begin = ccl_bufidx;

        /* Look-ahead to determine if negated */
        if (pattern[i+1] == '^')
        {
          re_compiled[j].type = INV_CHAR_CLASS;
          i += 1; /* Increment i to avoid including '^' in the char-buffer */
          if (pattern[i+1] == 0) /* incomplete pattern, missing non-zero char after '^' */
          {
            return 0;
          }
        }
        else
        {
          re_compiled[j].type = CHAR_CLASS;
        }

        /* Copy characters inside [..] to buffer */
        while (    (pattern[++i] != ']')
                && (pattern[i]   != '\0')) /* Missing ] */
        {
          if (pattern[i] == '\\')
          {
            if (ccl_bufidx >= MAX_CHAR_CLASS_LEN - 1)
            {
              //fputs("exceeded internal buffer!\n", stderr);
              return 0;
            }
            if (pattern[i+1] == 0) /* incomplete pattern, missing non-zero char after '\\' */
            {
              return 0;
            }
            ccl_buf[ccl_bufidx++] = pattern[i++];
          }
          else if (ccl_bufidx >= MAX_CHAR_CLASS_LEN)
          {
              //fputs("exceeded internal buffer!\n", stderr);
              return 0;
          }
          ccl_buf[ccl_bufidx++] = pattern[i];
        }
        if (ccl_bufidx >= MAX_CHAR_CLASS_LEN)
        {
            /* Catches cases such as [00000000000000000000000000000000000000][ */
            //fputs("exceeded internal buffer!\n", stderr);
            return 0;
        }
        /* Null-terminate string end */
        ccl_buf[ccl_bufidx++] = 0;
        re_compiled[j].u.ccl = &ccl_buf[buf_begin];
      } break;

      /* Other characters: */
      default:
      {
        re_compiled[j].type = CHAR;
        re_compiled[j].u.ch = c;
      } break;
    }
    /* no buffer-out-of-bounds access on invalid patterns - see https://github.com/kokke/tiny-regex-c/commit/1a279e04014b70b0695fba559a7c05d55e6ee90b */
    if (pattern[i] == 0)
    {
      return 0;
    }

    i += 1;
    j += 1;
  }

  /* 'UNUSED' is a sentinel used to indicate end-of-pattern */
  re_compiled[j].type = UNUSED;

  return (re_t)re_compiled;
}

static int match_digit(const char c)
{
  return ae_match(c, ae_is_digit);
}

static int match_alpha(const char c)
{
  return ae_match(c, ae_is_alpha);
}

static int match_space(const char c)
{
  return ae_match(c, ae_is_space);
}

static int match_alnum(const char c)
{
  return (c == '_') || match_alpha(c) || match_digit(c);
}

static int match_range(const char c, const char *str)
{
  return (    (c != '-')
           && (str[0] != '\0')
           && (str[0] != '-')
           && (str[1] == '-')
           && (str[2] != '\0')
           && (    (c >= str[0])
                && (c <= str[2])));
}

static int match_dot(const char c)
{
#if defined(RE_DOT_MATCHES_NEWLINE) && (RE_DOT_MATCHES_NEWLINE == 1)
  (void)c;
  return 1;
#else
  return c != '\n' && c != '\r';
#endif
}

static int is_meta(const char c)
{
  return  (c == 's') ||
          (c == 'S') ||
          (c == 'w') ||
          (c == 'W') ||
          (c == 'd') ||
          (c == 'D');
}

static int match_meta(const char c, const char *str)
{
  switch (str[0])
  {
    case 'd': return  match_digit(c);
    case 'D': return !match_digit(c);
    case 'w': return  match_alnum(c);
    case 'W': return !match_alnum(c);
    case 's': return  match_space(c);
    case 'S': return !match_space(c);

    default:  return (c == str[0]);
  }
}

static int match_class(const char c, const char *str)
{
  do
  {
    if (match_range(c, str))
    {
      return 1;
    }
    else if (str[0] == '\\')
    {
      /* Escape-char: increment str-ptr and match on next char */
      str += 1;
      if (match_meta(c, str))
      {
        return 1;
      }
      else if ((c == str[0]) && !is_meta(c))
      {
        return 1;
      }
    }
    else if (c == str[0])
    {
      if (c == '-')
      {
        return ((str[-1] == '\0') || (str[1] == '\0'));
      }
      else
      {
        return 1;
      }
    }
  }
  while (*str++ != '\0');

  return 0;
}

static int match_one(regex_t p, const char c)
{
  switch (p.type)
  {
    case DOT:            return  match_dot(c);
    case CHAR_CLASS:     return  match_class(c, (const char *)p.u.ccl);
    case INV_CHAR_CLASS: return !match_class(c, (const char *)p.u.ccl);
    case DIGIT:          return  match_digit(c);
    case NOT_DIGIT:      return !match_digit(c);
    case ALPHA:          return  match_alnum(c);
    case NOT_ALPHA:      return !match_alnum(c);
    case WHITESPACE:     return  match_space(c);
    case NOT_WHITESPACE: return !match_space(c);

    default:             return (p.u.ch == c);
  }
}

static int match_star(regex_t p, regex_t *pattern, const char *text, int *matchlength)
{
  int prelen = *matchlength;
  const char *prepoint = text;

  while ((text[0] != '\0') && match_one(p, *text))
  {
    text++;
    (*matchlength)++;
  }

  while (text >= prepoint)
  {
    if (match_pattern(pattern, text--, matchlength))
    {
      return 1;
    }

    (*matchlength)--;
  }

  *matchlength = prelen;
  return 0;
}

static int match_plus(regex_t p, regex_t *pattern, const char *text, int *matchlength)
{
  const char *prepoint = text;

  while ((text[0] != '\0') && match_one(p, *text))
  {
    text++;
    (*matchlength)++;
  }

  while (text > prepoint)
  {
    if (match_pattern(pattern, text--, matchlength))
    {
      return 1;
    }

    (*matchlength)--;
  }

  return 0;
}

static int match_question(regex_t p, regex_t* pattern, const char* text, int* matchlength)
{
  if (p.type == UNUSED)
  {
    return 1;
  }

  if (match_pattern(pattern, text, matchlength))
  {
    return 1;
  }

  if (*text && match_one(p, *text++))
  {
    if (match_pattern(pattern, text, matchlength))
    {
      (*matchlength)++;
      return 1;
    }
  }

  return 0;
}

#if 0

static int match_pattern(regex_t *pattern, const char *text, int *matchlength)
{
  int pre = *matchlength;
  if ((pattern[0].type == UNUSED) || (pattern[1].type == QUESTIONMARK))
  {
    return match_question(pattern[1], &pattern[2], text, matchlength);
  }
  else if (pattern[1].type == STAR)
  {
    return match_star(pattern[0], &pattern[2], text, matchlength);
  }
  else if (pattern[1].type == PLUS)
  {
    return match_plus(pattern[0], &pattern[2], text, matchlength);
  }
  else if ((pattern[0].type == END) && pattern[1].type == UNUSED)
  {
    return text[0] == '\0';
  }
  else if ((text[0] != '\0') && match_one(pattern[0], text[0]))
  {
    (*matchlength)++;
    return match_pattern(&pattern[1], text+1);
  }
  else
  {
    *matchlength = pre;
    return 0;
  }
}

#else

static int match_pattern(regex_t *pattern, const char *text, int *matchlength)
{
  int pre = *matchlength;
  do
  {
    if ((pattern[0].type == UNUSED) || (pattern[1].type == QUESTIONMARK))
    {
      return match_question(pattern[0], &pattern[2], text, matchlength);
    }
    else if (pattern[1].type == STAR)
    {
      return match_star(pattern[0], &pattern[2], text, matchlength);
    }
    else if (pattern[1].type == PLUS)
    {
      return match_plus(pattern[0], &pattern[2], text, matchlength);
    }
    else if ((pattern[0].type == END) && pattern[1].type == UNUSED)
    {
      return (text[0] == '\0');
    }
/*  Branching is not working properly
    else if (pattern[1].type == BRANCH)
    {
      return (match_pattern(pattern, text) || match_pattern(&pattern[2], text));
    }
*/

    (*matchlength)++;
  }
  while ((text[0] != '\0') && match_one(*pattern++, *text++));

  *matchlength = pre;
  return 0;
}

#endif
