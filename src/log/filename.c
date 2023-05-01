#include "style.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

void filename(char *buffer, const size_t size, const char *text, text_style_t *properties)
{
  const int style_cache = properties->style;

  char primary[TEXT_STYLE_MAXBUF];
  char reset[TEXT_STYLE_MAXBUF];

  text_style_t reset_properties;

  properties->style = UNDERLINE_TEXT_STYLE;

  memset(buffer, 0, size * sizeof(char));

  memset(&reset_properties, 0, sizeof(text_style_t));

  compile(reset, TEXT_STYLE_MAXBUF, &reset_properties);
  compile(primary, TEXT_STYLE_MAXBUF, properties);

  const char fmt[] = "%s%s%s";
  sprintf(buffer, fmt, primary, text, reset);

  properties->style = style_cache;
}
