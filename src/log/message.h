#ifndef X_LOG_MESSAGE_H
#define X_LOG_MESSAGE_H

#include "style.h"

#include <stddef.h>

void _message(char *buffer, const size_t size, const char *text, text_style_t *properties);

#endif/*X_LOG_MESSAGE_H*/
