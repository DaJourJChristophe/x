/**
 * @file stack.c
 * @author Da'Jour J. Christophe (dajour.christophe@gmail.com)
 * @brief Create a generic stack so that potions of this program can
 *        recreate the stack as a specific type.
 * @version 0.1
 * @date 2023-04-12
 *
 * @copyright Copyright (c) 2023 Da'Jour J. Christophe. All rights reserved.
 */
#include "common.h"
#include "ds/stack.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/**
 * @brief Insert an item into the stack.
 */
bool stack_push(stack_t *stack, void *item, size_t const size)
{
  if (stack->top > (stack->cap - 1))
  {
    return false;
  }

  memcpy(stack->data + (stack->top++ * stack->ofs), item, (size * stack->ofs));
  return true;
}

/**
 * @brief Remove an item from the top of the stack.
 */
void *stack_pop(stack_t *stack)
{
  if (stack->top == 0)
  {
    return NULL;
  }

  return stack->data + (--stack->top * stack->ofs);
}
