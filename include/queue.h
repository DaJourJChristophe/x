#ifndef X_SYNTAX_QUEUE_H
#define X_SYNTAX_QUEUE_H

#include "lexer.h"
#include "token.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_QUEUE    1024 /* The capacity of the queue buffer. */

// struct ring_buffer
// {
//   void *data;    /* A pointer to the data buffer.           */
//   size_t ofs;    /* The size of a single item in the queue. */

//   uint64_t w;    /* A writer iterator.                      */
//   uint64_t r;    /* A reader iterator.                      */
// };

struct ring_buffer
{
  void *data;    /* A pointer to the data buffer.           */
  size_t ofs;    /* The size of a single item in the queue. */
  size_t cap;    /* The capacity of the data buffer. */

  uint64_t w;    /* A writer iterator.                      */
  uint64_t r;    /* A reader iterator.                      */
};

/**
 * @brief Define a namespace for the ring-buffer data structure.
 */
// typedef struct ring_buffer ring_buffer_t;

/**
 * @brief Define a namespace for the generic queue data structure.
 */
typedef struct ring_buffer queue_t;

/**
 * @brief Define a namespace for the syntax_queue_t structure.
 */
typedef queue_t syntax_queue_t;

syntax_queue_t *syntax_queue_new(size_t const cap);

void syntax_queue_destroy(ring_buffer_t *queue);

bool syntax_queue_write(syntax_queue_t *queue, syntax_token_t *data);

void *syntax_queue_read(syntax_queue_t *queue);

#endif/*X_SYNTAX_QUEUE_H*/
