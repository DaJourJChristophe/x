#include "facts.h"

int precedence(int kind)
{
  switch (kind)
  {
    case BITWISE_XOR:       return 9;
    case EXPONENTIAL:       return 9;
    case STAR:              return 8;
    case DIVISION:          return 8;
    case MODULUS:           return 8;
    case ADDITION:          return 5;
    case SUBTRACTION:       return 5;
    case OPEN_PARENTHESIS:  return 0;
    case CLOSE_PARENTHESIS: return 0;
  }

  return (-1);
}
