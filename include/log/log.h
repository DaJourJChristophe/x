#ifndef X_LOG_LOG_H
#define X_LOG_LOG_H

#include "common.h"

struct levels
{
  int info;
  int debug;
  int deprecated;
  int warn;
  int error;
  int fatal;
};

static const struct levels levels = {
  .info = 0,
  .debug = 1,
  .deprecated = 2,
  .warn = 3,
  .error = 4,
  .fatal = 5,
};

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

void log_info(
  const char unused *message,
  const char unused *filename,
  const char unused *module,
  const char unused *class,
  const char unused *function,
  const int unused line,
  const int unused ch,
  const char unused *description
);

void log_debug(
  const char unused *message,
  const char unused *filename,
  const char unused *module,
  const char unused *class,
  const char unused *function,
  const int unused line,
  const int unused ch,
  const char unused *description
);

void log_deprecated(
  const char *message,
  const char *filename,
  const char *module,
  const char *class,
  const char *function,
  const int line,
  const int ch,
  const char *description
);

void log_warn(
  const char *message,
  const char *filename,
  const char *module,
  const char *class,
  const char *function,
  const int line,
  const int ch,
  const char *description
);

void log_error(
  const char *message,
  const char *filename,
  const char *module,
  const char *class,
  const char *function,
  const int line,
  const int ch,
  const char *description
);

void log_scream(
  const char *message,
  const char *filename,
  const char *module,
  const char *class,
  const char *function,
  const int line,
  const int ch,
  const char *description
);

static const log_function_t functions[] = {
  &log_info,
  &log_debug,
  &log_deprecated,
  &log_warn,
  &log_error,
  &log_scream,
};

static log_function_t function;

#define x_log(level, message, description) \
  function = functions[level]; \
  function( \
    message, \
    __FILE__, \
    __MODULE__, \
    "Logger", \
    __func__, \
    __LINE__, \
    0, \
    description \
  );

#endif/*X_LOG_LOG_H*/
