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
#include "ds/linked-list.h"
#include "expr.h"
#include "symbol-table.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

struct scope
{
  linked_list_t *data; /* A linked-list containing all of the scopes. */
};

typedef struct scope scope_t;

static void *scope_data_copy(void *data, const size_t unused size)
{
  return symbol_table_copy(data);
}

static void scope_data_free(void *data)
{
  symbol_table_destroy((symbol_table_t *)data);
}

scope_t *scope_new(void)
{
  scope_t *scope = NULL;
  scope = __malloc(sizeof(scope_t));
  scope->data = linked_list_new(
    &scope_data_copy,
    &scope_data_free
  );
  return scope;
}

void scope_destroy(scope_t *scope)
{
  linked_list_destroy(scope->data);
  __free(scope);
}

static void * _scope_search(const void *data, const size_t unused size)
{
  if (data == NULL)
  {
    // throw error - symbol table is null
  }

  return symbol_table_get((symbol_table_t *)data, "a", 1, NULL);
}

void *scope_get(scope_t *scope, const char unused *key, const size_t unused key_size)
{
  return linked_list_reverse_map(scope->data, &_scope_search);
}

void scope_add(scope_t *scope)
{
  symbol_table_t *table = symbol_table_new();
  linked_list_push(scope->data, table, HASHMAP_SIZE);
  symbol_table_destroy(table);
}

void scope_remove(scope_t *scope)
{
  linked_list_pop(scope->data);
}

void scope_set(scope_t *scope, char *symbol,
  syntax_expression_t *expr,
  const size_t symbol_length, const size_t expr_length)
{
  symbol_table_t *table = linked_list_poll(scope->data);
  symbol_table_add(table, symbol, expr, symbol_length, expr_length);
  linked_list_pop(scope->data);
  linked_list_push(scope->data, table, HASHMAP_SIZE);
  // symbol_table_destroy(table);
}

int main(void)
{
  scope_t *scope = scope_new();
  scope_add(scope);
  int *x = __malloc(sizeof(int));
  char *name = __malloc(2 * sizeof(char));
  syntax_expression_t *expr = __calloc(1, sizeof(syntax_expression_t));
  *x = 5;
  expr->data=x;
  expr->size=sizeof(int);
  strncpy(name, "a", 2);
  scope_set(scope, name, expr, 1, sizeof(int));
  expression_destroy(expr);
  // __free(x);
  // NOTE: Returns value but returns empty value.
  //       The problem is in the deep copies between the scope and the linked-list and symbol-table.
  printf("%d\n", *(int *)scope_get(scope, name, 1));
  __free(name);
  scope_remove(scope);
  exit(EXIT_SUCCESS);
  scope_destroy(scope);
  return EXIT_SUCCESS;
}
