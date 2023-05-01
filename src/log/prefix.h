#ifndef X_LOG_PREFIX_H
#define X_LOG_PREFIX_H

#include "style.h"

#include <stddef.h>

void prefix(char *buffer, const size_t size, const char *text, text_style_t *properties);

#endif/*X_LOG_PREFIX_H*/
