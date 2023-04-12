#ifndef X_GENERIC_STACK_H
#define X_GENERIC_STACK_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stddef.h>
#include <stdint.h>

#define MAXSTACK   5

/**
 * @brief Define a generic stack data structure.
 */
struct stack { void *data; uint64_t top; size_t ofs; };

/**
 * @brief Define a namespace for the generic stack data structure.
 */
typedef struct stack stack_t;

#ifdef __cplusplus
}
#endif

#endif/*X_GENERIC_STACK_H*/
