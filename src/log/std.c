#include "filename.h"
#include "hierarchy.h"
#include "message.h"
#include "position.h"
#include "prefix.h"
#include "style.h"
#include "timestamp.h"

#include <stdio.h>

void log_stdout(
  const char *prefix_text,
  const char *message,
  const char *filename_text,
  const char *module_text,
  const char *class_text,
  const char *function_text,
  const int line,
  const int ch,
  const char *description,
  text_style_t *prefix_properties
)
{
  char timestamp_buffer[4096];
  char filename_buffer[TEXT_STYLE_MAXBUF];
  char prefix_buffer[TEXT_STYLE_MAXBUF];
  char hierarchy_buffer[TEXT_STYLE_MAXBUF];
  char position_buffer[TEXT_STYLE_MAXBUF];
  char message_buffer[TEXT_STYLE_MAXBUF];
  char description_buffer[TEXT_STYLE_MAXBUF];

  filename(filename_buffer, TEXT_STYLE_MAXBUF, filename_text, prefix_properties);
  prefix(prefix_buffer, TEXT_STYLE_MAXBUF, prefix_text, prefix_properties);
  hierarchy(hierarchy_buffer, TEXT_STYLE_MAXBUF, module_text, class_text, function_text, prefix_properties);
  position(position_buffer, TEXT_STYLE_MAXBUF, line, ch, prefix_properties);
  _message(message_buffer, TEXT_STYLE_MAXBUF, message, prefix_properties);
  _message(description_buffer, TEXT_STYLE_MAXBUF, description, prefix_properties);

  timestamp(timestamp_buffer, prefix_properties);

  fprintf(stdout, "%s %s — %s — %s — %s — %s — %s.\n",

    prefix_buffer,
    timestamp_buffer,
    filename_buffer,
    hierarchy_buffer,
    position_buffer,
    message_buffer,
    description_buffer
  );
}

void log_stderr(
  const char *prefix_text,
  const char *message,
  const char *filename_text,
  const char *module_text,
  const char *class_text,
  const char *function_text,
  const int line,
  const int ch,
  const char *description,
  text_style_t *prefix_properties
)
{
  char timestamp_buffer[4096];
  char filename_buffer[TEXT_STYLE_MAXBUF];
  char prefix_buffer[TEXT_STYLE_MAXBUF];
  char hierarchy_buffer[TEXT_STYLE_MAXBUF];
  char position_buffer[TEXT_STYLE_MAXBUF];
  char message_buffer[TEXT_STYLE_MAXBUF];
  char description_buffer[TEXT_STYLE_MAXBUF];

  filename(filename_buffer, TEXT_STYLE_MAXBUF, filename_text, prefix_properties);
  prefix(prefix_buffer, TEXT_STYLE_MAXBUF, prefix_text, prefix_properties);
  hierarchy(hierarchy_buffer, TEXT_STYLE_MAXBUF, module_text, class_text, function_text, prefix_properties);
  position(position_buffer, TEXT_STYLE_MAXBUF, line, ch, prefix_properties);
  _message(message_buffer, TEXT_STYLE_MAXBUF, message, prefix_properties);
  _message(description_buffer, TEXT_STYLE_MAXBUF, description, prefix_properties);

  timestamp(timestamp_buffer, prefix_properties);

  fprintf(stderr, "%s %s — %s — %s — %s — %s — %s.\n",

    prefix_buffer,
    timestamp_buffer,
    filename_buffer,
    hierarchy_buffer,
    position_buffer,
    message_buffer,
    description_buffer
  );
}
