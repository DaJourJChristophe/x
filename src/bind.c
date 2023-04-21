#include "common.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node
{
  void *data;

  struct node *left;
  struct node *right;
};

typedef struct node node_t;

node_t *node_new(void *data)
{
  node_t *node = NULL;
  node = __calloc(1, sizeof(node_t));
  node->data = data;
  return node;
}

void node_destroy(node_t *node)
{
  __free(node);
}

bool bound_binary_tree_new(void *left, void *right)
{
  return left && right;
}


bool bound_unary_tree_new(void *left, void *right)
{
  return left && !right;
}

bool bound_numeric_tree_new(void *left, void *right)
{
  return !left && !right;
}

struct diagnostic
{
  char **data;
  size_t n;
};

typedef struct diagnostic diagnostic_t;

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
  strcpy(diagnostic->data[diagnostic->n++], errmsg);
}

#define DIAGNOSTIC_MAXBUF   4096

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

static const char invalid_binary_expression[] = "invalid binary expression";
static const char invalid_unary_expression[] = "invalid unary expression";
static const char invalid_numeric_expression[] = "invalid numeric literal";

int main(void)
{
  int a = 5;
  int b = 3;

  diagnostic_t *diagnostic = diagnostic_new();

  if (bound_binary_tree_new(&a, NULL) == false)
  {
    diagnostic_add(diagnostic, invalid_binary_expression);
  }
  if (bound_unary_tree_new(NULL, NULL) == false)
  {
    diagnostic_add(diagnostic, invalid_unary_expression);
  }
  if (bound_numeric_tree_new(&a, NULL) == false)
  {
    diagnostic_add(diagnostic, invalid_numeric_expression);
  }

  diagnostic_show(diagnostic);
  diagnostic_destroy(diagnostic);
  return EXIT_SUCCESS;
}
