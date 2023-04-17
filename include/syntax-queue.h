#ifndef X_SYNTAX_QUEUE_H
#define X_SYNTAX_QUEUE_H

#include "common.h"
#include "ds/queue.h"
#include "token.h"

#include <stddef.h>

/**
 * @brief Define a namespace for the syntax_queue_t structure.
 */
typedef queue_t syntax_queue_t;

static inline syntax_queue_t * always_inline syntax_queue_new(size_t const cap /* capacity */)
{
  return queue_new(cap, sizeof(syntax_token_t));
}

static inline bool always_inline syntax_queue_write(syntax_queue_t *queue, syntax_token_t *data)
{
  return queue_write(queue, data);
}

static inline syntax_token_t * always_inline syntax_queue_read(syntax_queue_t *queue)
{
  return (syntax_token_t *)queue_read(queue);
}

/**
 * @brief Deallocate the ring-buffer data-buffer and the ring-buffer
 *        structure.
 */
static inline void always_inline syntax_queue_destroy(syntax_queue_t *queue)
{
  queue_destroy(queue);
}

#endif/*X_SYNTAX_QUEUE_H*/
