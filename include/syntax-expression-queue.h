#ifndef X_SYNTAX_EXPRESSION_QUEUE_H
#define X_SYNTAX_EXPRESSION_QUEUE_H

#include "common.h"
#include "ds/queue.h"
#include "expr.h"
#include "token.h"

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Define a namespace for the syntax_expression_queue_t structure.
 */
typedef queue_t syntax_expression_queue_t;

static inline syntax_expression_queue_t * always_inline syntax_expression_queue_new(size_t const cap /* capacity */)
{
  return queue_new(cap, sizeof(syntax_expression_t));
}

static inline bool always_inline syntax_expression_queue_is_empty(syntax_expression_queue_t *queue)
{
  return queue_is_empty(queue);
}

/**
 * @brief Calculate the ring-buffer capacity by subtracting the writer
 *        position from the reader position and return to the end-user.
 */
static inline size_t always_inline queue_size(queue_t *buffer /* queue-buffer */)
{
  const uint64_t w = buffer->w;
  const uint64_t r = buffer->r;

  return w - r;
}

/**
 * @brief Return the ring-buffer capacity to the end-user.
 */
static inline size_t always_inline queue_capacity(queue_t *buffer)
{
  return buffer->cap;
}

static inline bool always_inline queue_is_full(queue_t *buffer)
{
  return queue_size(buffer) >= queue_capacity(buffer);
}

static bool syntax_expression_queue_write(syntax_expression_queue_t *queue, syntax_expression_t *expr)
{
  if (queue_is_full(queue))
  {
    return false;
  }

  const uint64_t w   = queue->w;
  const   size_t cap = queue->cap;
  void **ptr = queue->data;
  ptr[w % cap] = expression_copy(expr);
  queue->w++;
  return true;
}

static syntax_expression_t *syntax_expression_queue_read(syntax_expression_queue_t *queue)
{
  if (queue_is_empty(queue))
  {
    return NULL;
  }

  const   size_t cap = queue->cap;
  const uint64_t   r = queue->r;
  void **ptr = queue->data;
  syntax_expression_t *ret = expression_copy(ptr[r % cap]);
  expression_destroy(ptr[r % cap]);
  queue->r++;
  return ret;
}

/**
 * @brief Deallocate the ring-buffer data-buffer and the ring-buffer
 *        structure.
 */
static void syntax_expression_queue_destroy(syntax_expression_queue_t *queue)
{
  const   size_t cap = queue->cap;
  const uint64_t   w = queue->w;
        uint64_t   r = queue->r;

  void **ptr = queue->data;

  for (; r < w; r++)
  {
    expression_destroy(ptr[r % cap]);
  }

  __free(queue->data);
  __free(queue);
}

#endif/*X_SYNTAX_EXPRESSION_QUEUE_H*/
