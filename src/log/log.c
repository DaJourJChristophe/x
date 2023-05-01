#include "common.h"
#include "prefix.h"
#include "std.h"
#include "style.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void log_info(
  const char unused *message,
  const char unused *filename,
  const char unused *module,
  const char unused *class,
  const char unused *function,
  const int unused line,
  const int unused ch,
  const char unused *description
)
{
#if defined(VERBOSE)
  const char prefix_text[] = "info";
  text_style_t prefix_properties;
  memset(&prefix_properties, 0, sizeof(text_style_t));
  prefix_properties.color = GREY_TEXT_COLOR;
  log_stdout(
    prefix_text,
    message,
    filename,
    module,
    class,
    function,
    line,
    ch,
    description,
    &prefix_properties
  );
#endif/*VERBOSE*/
}

void log_debug(
  const char unused *message,
  const char unused *filename,
  const char unused *module,
  const char unused *class,
  const char unused *function,
  const int unused line,
  const int unused ch,
  const char unused *description
)
{
#if defined(NDEBUG)
  const char prefix_text[] = "debug";
  text_style_t prefix_properties;
  memset(&prefix_properties, 0, sizeof(text_style_t));
  prefix_properties.color = BLUE_TEXT_COLOR;
  log_stdout(
    prefix_text,
    message,
    filename,
    module,
    class,
    function,
    line,
    ch,
    description,
    &prefix_properties
  );
#endif/*NDEBUG*/
}

void log_deprecated(
  const char *message,
  const char *filename,
  const char *module,
  const char *class,
  const char *function,
  const int line,
  const int ch,
  const char *description
)
{
  const char prefix_text[] = "warn";
  text_style_t prefix_properties;
  memset(&prefix_properties, 0, sizeof(text_style_t));
  prefix_properties.style = OPACITY_TEXT_STYLE;
  prefix_properties.color = YELLOW_TEXT_COLOR;
  log_stdout(
    prefix_text,
    message,
    filename,
    module,
    class,
    function,
    line,
    ch,
    description,
    &prefix_properties
  );
}

void log_warn(
  const char *message,
  const char *filename,
  const char *module,
  const char *class,
  const char *function,
  const int line,
  const int ch,
  const char *description
)
{
  const char prefix_text[] = "warn";
  text_style_t prefix_properties;
  memset(&prefix_properties, 0, sizeof(text_style_t));
  prefix_properties.color = YELLOW_TEXT_COLOR;
  log_stdout(
    prefix_text,
    message,
    filename,
    module,
    class,
    function,
    line,
    ch,
    description,
    &prefix_properties
  );
}

void log_error(
  const char *message,
  const char *filename,
  const char *module,
  const char *class,
  const char *function,
  const int line,
  const int ch,
  const char *description
)
{
  const char prefix_text[] = "error";
  text_style_t prefix_properties;
  memset(&prefix_properties, 0, sizeof(text_style_t));
  prefix_properties.color = RED_TEXT_COLOR;
  log_stderr(
    prefix_text,
    message,
    filename,
    module,
    class,
    function,
    line,
    ch,
    description,
    &prefix_properties
  );
}

void log_scream(
  const char *message,
  const char *filename,
  const char *module,
  const char *class,
  const char *function,
  const int line,
  const int ch,
  const char *description
)
{
  const char prefix_text[] = "error";
  text_style_t prefix_properties;
  memset(&prefix_properties, 0, sizeof(text_style_t));
  prefix_properties.color = RED_TEXT_COLOR;
  prefix_properties.bg_color = RED_TEXT_BACKGROUND_COLOR;
  log_stderr(
    prefix_text,
    message,
    filename,
    module,
    class,
    function,
    line,
    ch,
    description,
    &prefix_properties
  );
}
