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
#include "lexer.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct ring_buffer
{
  syntax_token_t *data;    /* A pointer to the data buffer.    */
  size_t cap;              /* The capacity of the data buffer. */

  uint64_t w;              /* A writer iterator.               */
  uint64_t r;              /* A reader iterator.               */
};

/**
 * @brief Define a namespace for the ring-buffer structure.
 */
typedef struct ring_buffer ring_buffer_t;

/**
 * @brief Allocate a new ring-buffer structure and allocate the
 *        ring-buffer data-buffer based upon the capacity parameter
 *        provided by the end-user. Also, store the capacity as apart
 *        of the ring-buffer structure.
 */
ring_buffer_t *ring_buffer_new(size_t const cap)
{
  ring_buffer_t *buffer = NULL;
  buffer = __calloc(1, sizeof(ring_buffer_t));
  buffer->data = __calloc(cap, sizeof(syntax_token_t));
  buffer->cap = cap;
  return buffer;
}

/**
 * @brief Deallocate the ring-buffer data-buffer and the ring-buffer
 *        structure.
 */
void ring_buffer_destroy(ring_buffer_t *buffer)
{
  __free(buffer->data);
  __free(buffer);
}

/**
 * @brief Calculate the ring-buffer capacity by subtracting the writer
 *        position from the reader position and return to the end-user.
 */
static inline size_t always_inline ring_buffer_size(ring_buffer_t *buffer)
{
  return buffer->w - buffer->r;
}

/**
 * @brief Return the ring-buffer capacity to the end-user.
 */
static inline size_t always_inline ring_buffer_capacity(ring_buffer_t *buffer)
{
  return buffer->cap;
}

/**
 * @brief Check if the ring-buffer is full by comparing the ring-buffer
 *        size to the ring-buffer capacity. If the size is greater than
 *        or equal to the capacity, the ring-buffer is full, if not, the
 *        ring-buffer is not full.
 */
static bool ring_buffer_is_full(ring_buffer_t *buffer)
{
  return ring_buffer_size(buffer) >= ring_buffer_capacity(buffer);
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
bool ring_buffer_write(ring_buffer_t *buffer, syntax_token_t *data)
{
  if (ring_buffer_is_full(buffer))
  {
    return false;
  }

  memcpy((buffer->data + (buffer->w++ % buffer->cap)), data, sizeof(syntax_token_t));
  return true;
}

/**
 * @brief Check if the ring-buffer is empty by comparing the reader
 *        position and the writer position. If the reader is greater than
 *        or equal to the writer position the ring-buffer is empty, if
 *        not than the ring-buffer is not empty.
 */
static bool ring_buffer_is_empty(ring_buffer_t *buffer)
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
void *ring_buffer_read(ring_buffer_t *buffer)
{
  if (ring_buffer_is_empty(buffer))
  {
    return NULL;
  }

  return (buffer->data + (buffer->r++ % buffer->cap));
}

int main(void)
{
  ring_buffer_t *buffer = ring_buffer_new(3);

  uint64_t x = 1515;

  syntax_token_t token = {
    .type=3,
    .data=&x,
    .i=NULL,
    .j=NULL,
  };

  ring_buffer_write(buffer, &token);

  syntax_token_t *return_token = ring_buffer_read(buffer);

  if (return_token == NULL)
  {
    fprintf(stderr, "%s\n", "empty queue");
    exit(EXIT_FAILURE);
  }

  printf("type: %d\n", return_token->type);
  printf("data: %d\n", *(int *)return_token->data);

  ring_buffer_destroy(buffer);
  return EXIT_SUCCESS;
}
