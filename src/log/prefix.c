#include "style.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

void prefix(char *buffer, const size_t size, const char *text, text_style_t *properties)
{
  char primary[TEXT_STYLE_MAXBUF];
  char brackets[TEXT_STYLE_MAXBUF];
  char reset[TEXT_STYLE_MAXBUF];

  text_style_t bracket_properties;
  text_style_t reset_properties;

  bracket_properties.color = GREY_TEXT_COLOR;

  memset(buffer, 0, size * sizeof(char));

  memset(&bracket_properties, 0, sizeof(text_style_t));
  memset(&reset_properties, 0, sizeof(text_style_t));

  compile(brackets, TEXT_STYLE_MAXBUF, &bracket_properties);
  compile(reset, TEXT_STYLE_MAXBUF, &reset_properties);
  compile(primary, TEXT_STYLE_MAXBUF, properties);

  const char fmt[] = "%s[%s%s%s%s%s]%s";
  sprintf(buffer, fmt, brackets, reset, primary, text, reset, brackets, reset);
}
