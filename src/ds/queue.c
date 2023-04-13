/**
 * @file ring-buffer.c
 * @author Da'Jour J. Christophe (dajour.christophe@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-04-10
 *
 * @copyright Copyright (c) 2023 Da'Jour J. Christophe. All rights reserved.
 */
#include "common.h"
#include "ds/queue.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/**
 * @brief Calculate the ring-buffer capacity by subtracting the writer
 *        position from the reader position and return to the end-user.
 */
static inline size_t always_inline queue_size(queue_t *buffer /* queue-buffer */)
{
  return buffer->w - buffer->r;
}

/**
 * @brief Return the ring-buffer capacity to the end-user.
 */
static inline size_t always_inline queue_capacity(queue_t *buffer)
{
  return buffer->cap;
}

/**
 * @brief Check if the ring-buffer is full by comparing the ring-buffer
 *        size to the ring-buffer capacity. If the size is greater than
 *        or equal to the capacity, the ring-buffer is full, if not, the
 *        ring-buffer is not full.
 */
static bool queue_is_full(queue_t *buffer)
{
  return queue_size(buffer) >= queue_capacity(buffer);
}

/**
 * @brief Writer a syntax token into the ring-buffer data-buffer.
 *        First, check that the ring-buffer is not full, if so, return
 *        false to the end-user indicating a failed write. Then increment
 *        writer index and take the modulus by the capacity to get the
 *        writer position. Shift the data-buffer pointer by the writer
 *        position and copy the syntax token data into the syntax token
 *        in the data-buffer. Finally, return true to indicate to the
 *        end-user that the write was successful.
 */
bool queue_write(queue_t *buffer, void *data)
{
  if (queue_is_full(buffer))
  {
    return false;
  }

  memcpy((buffer->data + ((buffer->w++ * buffer->ofs) % buffer->cap)), data, buffer->ofs);
  return true;
}

/**
 * @brief Check if the ring-buffer is empty by comparing the reader
 *        position and the writer position. If the reader is greater than
 *        or equal to the writer position the ring-buffer is empty, if
 *        not than the ring-buffer is not empty.
 */
static bool queue_is_empty(queue_t *buffer)
{
  return buffer->r >= buffer->w;
}

/**
 * @brief Read from the ring-buffer and return a pointer to the syntax
 *        token stored in the ring-buffer. First, check if the ring-buffer
 *        is empty, if so, return null, indicating to the end-user that
 *        the ring-buffer is empty. If no, increment the index of the
 *        reader and than take the modulus by the capacity to get the
 *        reader position and than shift the data-buffer pointer by the
 *        reader position to get the next syntax token. Finally, return a
 *        pointer to the syntax token stored in the data-buffer.
 */
void *queue_read(queue_t *buffer)
{
  return queue_is_empty(buffer) ? NULL : (buffer->data + ((buffer->r++ * buffer->ofs) % buffer->cap));
}
