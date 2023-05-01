#ifndef X_LOG_LOG_H
#define X_LOG_LOG_H

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

void x_log(const int level, const char *message, const char *description);

#endif/*X_LOG_LOG_H*/
