#include "expr.h"
#include "ds/map.h"

typedef map_t symbol_table_t;

symbol_table_t *symbol_table_new(void)
{
  return map_new();
}

void symbol_table_destroy(symbol_table_t *table)
{
  map_destroy(table);
}

void symbol_table_add(symbol_table_t *table, char *symbol, syntax_expression_t *expr, const size_t symbol_length, const size_t data_length)
{
  map_set(table, symbol, expr, symbol_length, data_length);
}

void *symbol_table_get(symbol_table_t *table, char *symbol, const size_t symbol_length, size_t *n)
{
  return map_get(table, symbol, symbol_length, n ? n : NULL);
}
