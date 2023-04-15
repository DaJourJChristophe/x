#ifndef X_BINARY_TREE_H
#define X_BINARY_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct binary_tree
{
  void **data;
  size_t ofs;
  size_t cap;
};

typedef struct binary_tree binary_tree_t;

binary_tree_t *binary_tree_new(size_t const cap /* capacity */,
                               size_t const ofs /* offset   */);

void binary_tree_destroy(binary_tree_t *tree);

void binary_tree_set_root(binary_tree_t *tree, const void *data);

void *binary_tree_get_root(binary_tree_t *tree);

void binary_tree_set_left(binary_tree_t *tree, const uint64_t i, const void *data);

void binary_tree_set_right(binary_tree_t *tree, const uint64_t i, const void *data);

bool binary_tree_search(binary_tree_t *tree, void *data);

void binary_tree_insert(binary_tree_t *tree, void *data);

#endif/*X_BINARY_TREE_H*/
