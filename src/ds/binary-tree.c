#include "common.h"
#include "ds/binary-tree.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

binary_tree_t *binary_tree_new(size_t const cap /* capacity */,
                               size_t const ofs /* offset   */)
{
  binary_tree_t *tree = NULL;
  tree = __malloc(sizeof(binary_tree_t));
  tree->data = __calloc(cap, sizeof(uintptr_t));
  tree->ofs = ofs;
  tree->cap = cap;
  return tree;
}

void binary_tree_destroy(binary_tree_t *tree)
{
  __free(tree->data);
  __free(tree);
}

void binary_tree_set_root(binary_tree_t *tree, const void *data)
{
  *tree->data = __malloc(tree->ofs);
  memcpy(*tree->data, data, tree->ofs);
}

void *binary_tree_get_root(binary_tree_t *tree)
{
  return *tree->data;
}

void binary_tree_set_left(binary_tree_t *tree, const uint64_t i, const void *data)
{
  void **pos = tree->data + ((2 * i + 1) * tree->ofs);
  *pos = __malloc(tree->ofs);
  memcpy(*pos, data, tree->ofs);
}

void binary_tree_set_right(binary_tree_t *tree, const uint64_t i, const void *data)
{
  void **pos = tree->data + ((2 * i + 2) * tree->ofs);
  *pos = __malloc(tree->ofs);
  memcpy(*pos, data, tree->ofs);
}

bool binary_tree_search(binary_tree_t *tree, void *data)
{
  void **pos;
  uint64_t i;
  int res;

  pos = NULL;
  i = 0;
  res = 0;

  while (i < tree->cap)
  {
    pos = tree->data + (i * sizeof(uintptr_t));

    if (*pos == NULL)
    {
      break;
    }

    res = memcmp(*pos, data, tree->ofs);

    if (res == 0)
    {
      return true;
    }

    if (res > 0)
    {
      i = 2 * i + 1;
    }
    else
    {
      i = 2 * i + 2;
    }
  }

  return false;
}

void binary_tree_insert(binary_tree_t *tree, void *data)
{
  void **pos;
  uint64_t i;
  int res;

  pos = NULL;
  i = 0;
  res = 0;

  while (i < tree->cap)
  {
    pos = tree->data + (i * sizeof(uintptr_t));

    if (*pos == NULL)
    {
      *pos = __malloc(tree->ofs);
      memcpy(*pos, data, tree->ofs);
      break;
    }

    res = memcmp(*pos, data, tree->ofs);

    if (res == 0)
    {
      break;
    }

    if (res > 0)
    {
      i = 2 * i + 1;
    }
    else
    {
      i = 2 * i + 2;
    }
  }
}
