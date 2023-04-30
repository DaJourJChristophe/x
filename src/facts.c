#include "facts.h"

int precedence(int kind)
{
  switch (kind)
  {
    case BITWISE_XOR_TOK:       return 9;
    case EXPONENTIAL_TOK:       return 9;
    case STAR_TOK:              return 8;
    case SLASH_TOK:             return 8;
    case MODULUS_TOK:           return 8;
    case PLUS_TOK:              return 5;
    case HYPHEN_TOK:            return 5;
    case OPEN_PARENTHESIS_TOK:  return 0;
    case CLOSE_PARENTHESIS_TOK: return 0;
  }

  return (-1);
}
