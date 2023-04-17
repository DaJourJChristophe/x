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
#include <stdint.h>
#include <string.h>

/**
 * @brief Allocate a new stack data structure to the CPU stack and
 *        return a CPU pointer.
 */
stack_t *stack_new(size_t const cap /* capacity */,
                   size_t const ofs /* offset   */)
{
  stack_t *stack = NULL;
  stack = __calloc(1, sizeof(stack_t));
  stack->data = __calloc(cap, sizeof(uintptr_t));

  stack->ofs  = ofs;
  stack->cap  = cap;

  return stack;
}

void stack_destroy(stack_t *stack)
{
  const uint64_t top = stack->top;
  void **ptr = stack->data;

  for (uint64_t i = 0; i < top; i++)
  {
    __free(ptr[i]);
  }

  __free(stack->data);
  __free(stack);
}

void *stack_peek(stack_t *stack)
{
  const uint64_t top = stack->top;

  if (top == 0)
  {
    return NULL;
  }

  const void **ptr = (const void **)stack->data;
  const size_t ofs = stack->ofs;
  void *data = __malloc(ofs);
  memcpy(data, ptr[(top - 1)], ofs);
  return data;
}

/**
 * @brief Insert an item into the stack.
 */
bool stack_push(stack_t *stack, void *data)
{
  const uint64_t top = stack->top;
  const   size_t cap = stack->cap;

  if (top > (cap - 1))
  {
    return false;
  }

  const size_t ofs = stack->ofs;
  void **ptr = stack->data;
  ptr[top] = __malloc(ofs);
  memcpy(ptr[top], data, ofs);
  stack->top++;
  return true;
}

/**
 * @brief Remove an item from the top of the stack.
 */
void *stack_pop(stack_t *stack)
{
  uint64_t top = stack->top;

  if (top == 0)
  {
    return NULL;
  }

  const size_t ofs = stack->ofs;
  void **ptr = stack->data;
  void *data = __malloc(ofs);
  stack->top--;
  top = stack->top;
  memcpy(data, ptr[top], ofs);
  __free(ptr[top]);
  ptr[top] = NULL;
  return data;
}
