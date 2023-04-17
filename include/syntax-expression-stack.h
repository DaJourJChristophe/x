#ifndef X_SYNTAX_EXPRESSION_STACK_H
#define X_SYNTAX_EXPRESSION_STACK_H

#include "ds/stack.h"
#include "expr.h"
#include "token.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef stack_t syntax_expression_stack_t;

static inline syntax_expression_stack_t * always_inline syntax_expression_stack_new(size_t const cap /* capacity */)
{
  return stack_new(cap, sizeof(syntax_expression_t));
}

static void syntax_expression_stack_destroy(syntax_expression_stack_t *stack)
{
  const uint64_t top = stack->top;
  syntax_expression_t **ptr = (syntax_expression_t **)stack->data;

  for (uint64_t i = 0; i < top; i++)
  {
    expression_destroy(ptr[i]);
  }

  __free(stack->data);
  __free(stack);
}

static syntax_expression_t *syntax_expression_stack_peek(syntax_expression_stack_t *stack)
{
  const uint64_t top = stack->top;

  if (top == 0)
  {
    return NULL;
  }

  const syntax_expression_t **ptr = (const syntax_expression_t **)stack->data;
  return expression_copy((syntax_expression_t *)ptr[(top - 1)]);
}

static bool syntax_expression_stack_push(syntax_expression_stack_t *stack, syntax_expression_t *expr)
{
  const uint64_t top = stack->top;
  const   size_t cap = stack->cap;

  if (top > (cap - 1))
  {
    return false;
  }

  void **ptr = stack->data;
  ptr[top] = expression_copy(expr);
  stack->top++;
  return true;
}

static syntax_expression_t *syntax_expression_stack_pop(syntax_expression_stack_t *stack)
{
  uint64_t top = stack->top;

  if (top == 0)
  {
    return NULL;
  }

  syntax_expression_t **ptr = (syntax_expression_t **)stack->data;
  stack->top--;
  top = stack->top;
  syntax_expression_t *ret = expression_copy(ptr[top]);
  expression_destroy(ptr[top]);
  ptr[top] = NULL;
  return ret;
}

#endif/*X_SYNTAX_EXPRESSION_STACK_H*/
