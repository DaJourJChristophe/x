#include "common.h"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define MAXSTACK   5

struct stack { void *data; uint64_t top; size_t ofs; };

typedef struct stack stack_t;

inline stack_t always_inline *stack_new(void *data /* stack-pointer */, size_t const ofs /* offset */)
{
  stack_t stack = {.data=data, .ofs=ofs, .top=0}, *stack_ptr = &stack;
  return stack_ptr;
}

bool stack_push(stack_t *stack, void *item, size_t const size)
{
  if (stack->top > (MAXSTACK - 1))
  {
    return false;
  }

  memcpy(stack->data + (stack->top++ * stack->ofs), item, size);
  return true;
}

void *stack_pop(stack_t *stack)
{
  if (stack->top == 0)
  {
    return NULL;
  }

  return stack->data + (--stack->top * stack->ofs);
}

int main(void)
{
  int data[MAXSTACK];
  stack_t *stack = stack_new(data, sizeof(int));

  int x = 1;
  if (stack_push(stack, &x, sizeof(int)) == false)
  {
    fprintf(stderr, "stack is full");
  }
  x = 2;
  if (stack_push(stack, &x, sizeof(int)) == false)
  {
    fprintf(stderr, "stack is full");
  }
  x = 3;
  if (stack_push(stack, &x, sizeof(int)) == false)
  {
    fprintf(stderr, "stack is full");
  }
  x = 4;
  if (stack_push(stack, &x, sizeof(int)) == false)
  {
    fprintf(stderr, "stack is full");
  }
  x = 5;
  if (stack_push(stack, &x, sizeof(int)) == false)
  {
    fprintf(stderr, "stack is full");
  }

  printf("%d\n", *(int *)stack_pop(stack));
  printf("%d\n", *(int *)stack_pop(stack));
  printf("%d\n", *(int *)stack_pop(stack));
  printf("%d\n", *(int *)stack_pop(stack));
  printf("%d\n", *(int *)stack_pop(stack));

  // stack_destroy(stack);
  return EXIT_SUCCESS;
}
