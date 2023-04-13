#ifndef X_SYNTAX_EXPRESSION_QUEUE_H
#define X_SYNTAX_EXPRESSION_QUEUE_H

#include "common.h"
#include "ds/queue.h"
#include "token.h"
#include "expression.h"

#include <stddef.h>

/**
 * @brief Define a namespace for the syntax_expression_queue_t structure.
 */
typedef queue_t syntax_expression_queue_t;

static inline syntax_expression_queue_t * always_inline syntax_expression_queue_new(size_t const cap /* capacity */)
{
  syntax_expression_t *data = NULL;
  data = (syntax_expression_t *)__calloc(cap, sizeof(syntax_expression_t));
  return queue_new(data, cap, sizeof(syntax_expression_t));
}

static inline bool always_inline syntax_expression_queue_write(syntax_expression_queue_t *queue, syntax_expression_t *data)
{
  return queue_write(queue, data);
}

static inline syntax_expression_t * always_inline syntax_expression_queue_read(syntax_expression_queue_t *queue)
{
  return (syntax_expression_t *)queue_read(queue);
}

/**
 * @brief Deallocate the ring-buffer data-buffer and the ring-buffer
 *        structure.
 */
static inline void always_inline syntax_expression_queue_destroy(syntax_expression_queue_t *buffer)
{
  syntax_expression_t *expression = NULL;

  while ((expression = syntax_expression_queue_read(buffer)) != NULL)
  {
    __free(expression->value->data);
  }

  __free(buffer->data);
  __free(buffer);
}

#endif/*X_SYNTAX_EXPRESSION_QUEUE_H*/
