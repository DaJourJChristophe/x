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
  void **data;     /* A pointer to the data buffer.           */
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
stack_t *stack_new(size_t const cap /* capacity */,
                   size_t const ofs /* offset   */);

void stack_destroy(stack_t *stack);

void *stack_peek(stack_t *stack);

bool stack_push(stack_t *stack, void *data);

void *stack_pop(stack_t *stack);

#ifdef __cplusplus
}
#endif

#endif/*X_GENERIC_STACK_H*/
