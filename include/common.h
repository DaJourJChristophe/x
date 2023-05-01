#ifndef X_COMMON_H
#define X_COMMON_H

#include <stdio.h>
#include <stdlib.h>

#ifndef always_inline
#define always_inline __attribute__ ((always_inline))
#endif/*always_inline*/

#ifndef unused
#define unused __attribute__ ((unused))
#endif/*unused*/

#define INTERN_T static

#define INLINE_VOID_T inline void always_inline

static inline void always_inline warn(const char *message,
  const char *file_name, const char *func_name)
{
  const char fmt[] = "An error occurred in %s:\n  %s(): %s\n\nAborted\n";
  fprintf(stderr, fmt, file_name, func_name, message);
  exit(EXIT_FAILURE);
}

static inline void always_inline die(const char *message,
  const char *file_name, const char *func_name)
{
  warn(file_name, func_name, message);
  exit(EXIT_FAILURE);
}

static inline void always_inline * __malloc(size_t const n)
{
  void *p = NULL;
  p = malloc(n);
  if (p == NULL)
  {
    const char errmsg[] = "memory error";
    die(errmsg, __FILE__, __func__);
  }
  return p;
}

static inline void always_inline * __calloc(size_t const n, size_t const size)
{
  void *p = NULL;
  p = calloc(n, size);
  if (p == NULL)
  {
    const char errmsg[] = "memory error";
    die(errmsg, __FILE__, __func__);
  }
  return p;
}

static inline void always_inline __free(void *p)
{
  if (p != NULL)
  {
    free(p);
    p = NULL;
  }
}

#endif/*X_COMMON_H*/
