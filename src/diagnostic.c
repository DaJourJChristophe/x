#include "common.h"
#include "diagnostic.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

const char invalid_assignment_expression[] = "invalid assignment expression";
const char invalid_binary_expression[] = "invalid binary expression";
const char invalid_unary_expression[] = "invalid unary expression";
const char invalid_numeric_expression[] = "invalid numeric literal";

diagnostic_t *diagnostic_new(void)
{
  diagnostic_t *diagnostic = NULL;
  diagnostic = __calloc(1, sizeof(diagnostic_t));
  diagnostic->data = __calloc(16, sizeof(char *));
  return diagnostic;
}

void diagnostic_destroy(diagnostic_t *diagnostic)
{
  const size_t n = diagnostic->n;
  char **data = diagnostic->data;

  for (uint64_t i = 0; i < n; i++)
  {
    __free(data[i]);
  }

  __free(diagnostic->data);
  __free(diagnostic);
}

bool diagnostic_add(diagnostic_t *diagnostic, char const *errmsg)
{
  diagnostic->data[diagnostic->n] = __calloc(128, sizeof(char));
  return NULL != strcpy(diagnostic->data[diagnostic->n++], errmsg);
}

void diagnostic_show(diagnostic_t *diagnostic)
{
  const size_t n = diagnostic->n;
  char **data = diagnostic->data;

  char buffer[DIAGNOSTIC_MAXBUF];
  memset(buffer, 0, DIAGNOSTIC_MAXBUF * sizeof(char));

  for (uint64_t i = 0; i < n; i++)
  {
    strcat(buffer, data[i]);
    strcat(buffer, "\n");
  }

  printf("%s", buffer);
}

bool diagnostic_is_empty(diagnostic_t *diagnostic)
{
  return diagnostic->n == 0;
}
