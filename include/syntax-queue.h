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
  syntax_token_t data[cap];
  return queue_new(data, cap, sizeof(syntax_token_t));
}

static inline bool always_inline syntax_queue_write(syntax_queue_t *queue, syntax_token_t *data)
{
  return queue_write(queue, data);
}

static inline void * always_inline syntax_queue_read(syntax_queue_t *queue)
{
  return queue_read(queue);
}

/**
 * @brief Deallocate the ring-buffer data-buffer and the ring-buffer
 *        structure.
 */
static inline void always_inline syntax_queue_destroy(syntax_queue_t *buffer)
{
  syntax_token_t *token = NULL;

  do
  {
    token = syntax_queue_read(buffer);
    if (token != NULL)
    {
      __free(token->data);
    }
  }
  while (token != NULL);
}

#endif/*X_SYNTAX_QUEUE_H*/
