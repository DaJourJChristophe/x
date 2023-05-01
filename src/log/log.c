#include "common.h"
#include "prefix.h"
#include "std.h"
#include "style.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void log_info(
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
}

static void log_debug(
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
}

static void log_deprecated(
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

static void log_warn(
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

static void log_error(
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

static void log_scream(
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

typedef void (*log_function_t)(
  const char *,
  const char *,
  const char *,
  const char *,
  const char *,
  const int,
  const int,
  const char *
);

static const log_function_t functions[] = {
  &log_info,
  &log_debug,
  &log_deprecated,
  &log_warn,
  &log_error,
  &log_scream,
};

enum
{
  LOG_INFO,
  LOG_DEBUG,
  LOG_DEPRECATED,
  LOG_WARN,
  LOG_ERROR,
  LOG_FATAL,
};

void x_log(const int level, const char *message, const char *description)
{
  log_function_t function;
  function = functions[level];
  function(
    message,
    "logger.py",
    "Logger",
    "Logger",
    "print",
    12, 36,
    description
  );
}

// int main(void)
// {
//   x_log(levels.info, "This is a Hello, World message from the Logger module", "This is a Hello, World message from the Logger module");
//   x_log(levels.debug, "This is a Hello, World message from the Logger module", "This is a Hello, World message from the Logger module");
//   x_log(levels.deprecated, "This is a Hello, World message from the Logger module", "This is a Hello, World message from the Logger module");
//   x_log(levels.warn, "This is a Hello, World message from the Logger module", "This is a Hello, World message from the Logger module");
//   x_log(levels.error, "This is a Hello, World message from the Logger module", "This is a Hello, World message from the Logger module");
//   x_log(levels.fatal, "This is a Hello, World message from the Logger module", "This is a Hello, World message from the Logger module");
//   return EXIT_SUCCESS;
// }
