#ifndef X_DIAGNOSTIC_H
#define X_DIAGNOSTIC_H

#include <stdbool.h>
#include <stddef.h>

#define DIAGNOSTIC_MAXBUF   4096

struct diagnostic
{
  char **data;
  size_t n;
};

typedef struct diagnostic diagnostic_t;

extern const char invalid_binary_expression[128];
extern const char invalid_unary_expression[128];
extern const char invalid_numeric_expression[128];

#define DIAGNOSTIC_INVALID_BINARY_EXPRESSION  invalid_binary_expression
#define DIAGNOSTIC_INVALID_UNARY_EXPRESSION   invalid_unary_expression
#define DIAGNOSTIC_INVALID_NUMERIC_EXPRESSION invalid_numeric_expression

diagnostic_t *diagnostic_new(void);

void diagnostic_destroy(diagnostic_t *diagnostic);

bool diagnostic_add(diagnostic_t *diagnostic, char const *errmsg);

void diagnostic_show(diagnostic_t *diagnostic);

bool diagnostic_is_empty(diagnostic_t *diagnostic);

extern diagnostic_t *diagnostics;

#endif/*X_DIAGNOSTIC_H*/
