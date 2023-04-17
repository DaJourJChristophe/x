#ifndef X_QUEUE_H
#define X_QUEUE_H

#include "common.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DEFAULT_QUEUESIZE    512 /* The capacity of the queue buffer. */

struct ring_buffer
{
  void **data;   /* A pointer to the data buffer.           */
  size_t ofs;    /* The size of a single item in the queue. */
  size_t cap;    /* The capacity of the data buffer.        */
  uint64_t w;    /* A writer iterator.                      */
  uint64_t r;    /* A reader iterator.                      */
};

/**
 * @brief Define a namespace for the generic queue data structure.
 */
typedef struct ring_buffer queue_t;

/**
 * @brief Allocate a new ring-buffer structure and allocate the
 *        ring-buffer data-buffer based upon the capacity parameter
 *        provided by the end-user. Also, store the capacity as apart
 *        of the ring-buffer structure.
 */
queue_t *queue_new(size_t const cap /* capacity */,
                   size_t const ofs /* offset   */);

void queue_destroy(queue_t *buffer);

bool queue_write(queue_t *buffer, void *data);

bool queue_is_empty(queue_t *buffer);

void *queue_read(queue_t *buffer);

#endif/*X_QUEUE_H*/
