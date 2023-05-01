#include "style.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief
 */
void compile(char *buffer, const size_t size, text_style_t *style)
{
  memset(buffer, 0, size * sizeof(char));

  if (style->bg_color != 0)
  {
    const char fmt[] = "\033[%d;%d;%dm";
    sprintf(buffer, fmt, style->style, style->color, style->bg_color);
  }
  else
  {
    const char fmt[] = "\033[%d;%dm";
    sprintf(buffer, fmt, style->style, style->color);
  }
}
