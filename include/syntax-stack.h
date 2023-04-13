#ifndef X_SYNTAX_STACK_H
#define X_SYNTAX_STACK_H

#include "ds/stack.h"
#include "token.h"

#include <stdbool.h>
#include <stddef.h>

typedef stack_t syntax_stack_t;

static inline syntax_stack_t * always_inline syntax_stack_new(size_t const cap /* capacity */)
{
  syntax_token_t *data = NULL;
  data = __calloc(cap, sizeof(syntax_token_t));
  return stack_new(data, cap, sizeof(syntax_token_t));
}

static inline void always_inline syntax_stack_destroy(syntax_stack_t *stack)
{
  __free(stack->data);
  __free(stack);
}

static inline syntax_token_t * always_inline syntax_stack_peek(syntax_stack_t *stack)
{
  return (syntax_token_t *)stack_peek(stack);
}

static inline bool always_inline syntax_stack_push(syntax_stack_t *stack, syntax_token_t *item)
{
  return stack_push(stack, item);
}

static inline syntax_token_t * always_inline syntax_stack_pop(syntax_stack_t *stack)
{
  return (syntax_token_t *)stack_pop(stack);
}

#endif/*X_SYNTAX_STACK_H*/
