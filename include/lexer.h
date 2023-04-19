#ifndef X_LEXER_H
#define X_LEXER_H

#ifdef __cplusplus
extern "C"{
#endif

#include "syntax-queue.h"

syntax_queue_t *compile(const char *filepath);

syntax_queue_t *compile_line(const char *data);

#ifdef __cplusplus
}
#endif

#endif/*X_LEXER_H*/
