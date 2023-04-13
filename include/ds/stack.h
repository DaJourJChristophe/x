#ifndef X_GENERIC_STACK_H
#define X_GENERIC_STACK_H

#ifdef __cplusplus
extern "C"{
#endif

#include "common.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DEFAULT_STACKSIZE   512

/**
 * @brief Define a generic stack data structure.
 */
struct stack
{
  uint8_t *data;   /* A pointer to the data buffer.           */
  size_t ofs;      /* The size of a single item on the stack. */
  size_t cap;      /* The capacity of the data buffer.        */
  uint64_t top;    /* An interator pointing to the stack top. */
};

/**
 * @brief Define a namespace for the generic stack data structure.
 */
typedef struct stack stack_t;

/**
 * @brief Allocate a new stack data structure to the CPU stack and
 *        return a CPU pointer.
 */
static inline stack_t always_inline *stack_new(void *data /* data-pointer */, size_t const cap /* capacity */, size_t const ofs /* offset */)
{
  stack_t  stack_obj = {.data=data, .ofs=ofs, .cap=cap, .top=0};
  stack_t *stack_ptr = &stack_obj;
  return   stack_ptr;
}

bool stack_push(stack_t *stack, void *item, size_t const size);

void *stack_pop(stack_t *stack);

#ifdef __cplusplus
}
#endif

#endif/*X_GENERIC_STACK_H*/
