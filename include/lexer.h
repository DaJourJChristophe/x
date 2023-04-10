#ifndef X_LEXER_H
#define X_LEXER_H

#ifdef __cplusplus
extern "C"{
#endif

#include "queue.h"

syntax_queue_t *compile(const char *filepath);

#ifdef __cplusplus
}
#endif

#endif/*X_LEXER_H*/
