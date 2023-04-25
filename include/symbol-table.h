#ifndef X_SYMBOL_TABLE_H
#define X_SYMBOL_TABLE_H

#include "expr.h"
#include "ds/map.h"

#include <stddef.h>

typedef map_t symbol_table_t;

symbol_table_t *symbol_table_new(void);

symbol_table_t *symbol_table_copy(symbol_table_t *table);

void symbol_table_destroy(symbol_table_t *table);

void symbol_table_add(symbol_table_t *table, char *symbol, syntax_expression_t *expr, const size_t symbol_length, const size_t data_length);

void *symbol_table_get(symbol_table_t *table, char *symbol, const size_t symbol_length, size_t *n);

#endif/*X_SYMBOL_TABLE_H*/
