#ifndef X_EVAL_H
#define X_EVAL_H

#include "expr.h"
#include "symbol-table.h"

void eval(syntax_expression_t *root, symbol_table_t *table);

#endif/*X_EVAL_H*/
