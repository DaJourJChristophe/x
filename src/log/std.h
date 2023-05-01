#ifndef X_LOG_STD_H
#define X_LOG_STD_H

#include "style.h"

void log_stdout(const char *prefix_text,
  const char *message,
  const char *filename_text,
  const char *module_text,
  const char *class_text,
  const char *function_text,
  const int line,
  const int ch,
  const char *description,
  text_style_t *prefix_properties
);

void log_stderr(const char *prefix_text,
  const char *message,
  const char *filename_text,
  const char *module_text,
  const char *class_text,
  const char *function_text,
  const int line,
  const int ch,
  const char *description,
  text_style_t *prefix_properties
);

#endif/*X_LOG_STD_H*/
