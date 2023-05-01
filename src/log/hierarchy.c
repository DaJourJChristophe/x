#include "style.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

void hierarchy(
  char *buffer,
  const size_t size,
  const char *module,
  const char *class,
  const char *function,
  text_style_t *properties
)
{
  const int style_cache = properties->style;

  char primary[TEXT_STYLE_MAXBUF];
  char reset[TEXT_STYLE_MAXBUF];

  text_style_t reset_properties;

  properties->style = UNDERLINE_TEXT_STYLE;

  memset(buffer, 0, size * sizeof(char));

  memset(&reset_properties, 0, sizeof(text_style_t));

  log_compile(reset, TEXT_STYLE_MAXBUF, &reset_properties);
  log_compile(primary, TEXT_STYLE_MAXBUF, properties);

  const char fmt[] = "%s%s.%s.%s%s";
  sprintf(buffer, fmt, primary, module, class, function, reset);

  properties->style = style_cache;
}
