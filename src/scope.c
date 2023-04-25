/**
 * @file scope.c
 * @author Da'Jour J. Christophe (dajour.christophe@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-04-24
 *
 * @copyright Copyright (c) 2023 Da'Jour J. Christophe. All rights reserved.
 */
#include "common.h"
#include "symbol-table.h"

#include <stddef.h>

struct scope
{
  symbol_table_t *top; /* A pointer to the global scope. */
};

typedef struct scope scope_t;

scope_t *scope_new(void)
{
  scope_t *scope = NULL;
  scope = __malloc(sizeof(scope_t));
  scope->top = symbol_table_new();
  return scope;
}

void scope_destroy(scope_t *scope)
{
  symbol_table_destroy(scope->global_scope);
  __free(scope);
}
