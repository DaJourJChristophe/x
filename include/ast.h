#ifndef X_ABSTRACT_SYNTAX_TREE_H
#define X_ABSTRACT_SYNTAX_TREE_H

#include "expression.h"

struct abstract_syntax_tree
{
  syntax_expression_t *root;
};

typedef struct abstract_syntax_tree abstract_syntax_tree_t;

abstract_syntax_tree_t *abstract_syntax_tree_new(void);

void abstract_syntax_tree_destroy(abstract_syntax_tree_t *tree);

void printLevelOrder(syntax_expression_t *root);

void abstract_syntax_tree_insert(abstract_syntax_tree_t *tree, int kind, syntax_token_t *token);

#endif/*X_ABSTRACT_SYNTAX_TREE_H*/
