#include "common.h"
#include "ds/queue.h"
#include "syntax-queue.h"
#include "token.h"

inline syntax_queue_t * always_inline syntax_queue_new(size_t const cap /* capacity */)
{
  syntax_token_t data[cap];
  return queue_new(data, sizeof(syntax_token_t));
}

inline bool always_inline syntax_queue_write(syntax_queue_t *queue, syntax_token_t *data)
{
  return queue_write(queue, data);
}

inline void * always_inline syntax_queue_read(syntax_queue_t *queue)
{
  return ring_buffer_read(queue);
}
