#ifndef X_SYNTAX_QUEUE_H
#define X_SYNTAX_QUEUE_H

#include "ds/queue.h"
#include "token.h"

#include <stddef.h>

/**
 * @brief Define a namespace for the syntax_queue_t structure.
 */
typedef queue_t syntax_queue_t;

syntax_queue_t *syntax_queue_new(size_t const cap);

bool syntax_queue_write(syntax_queue_t *queue, syntax_token_t *data);

void *syntax_queue_read(syntax_queue_t *queue);

#endif/*X_SYNTAX_QUEUE_H*/
