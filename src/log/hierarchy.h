#ifndef X_LOG_HIERARCHY_H
#define X_LOG_HIERARCHY_H

#include "style.h"

void hierarchy(char *buffer,
  const size_t size,
  const char *module,
  const char *class,
  const char *function,
  text_style_t *properties
);

#endif/*X_LOG_HIERARCHY_H*/
