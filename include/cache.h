#ifndef X_CACHE_H
#define X_CACHE_H

#include "token.h"

struct splay_tree_node
{
  syntax_token_t *data;

  struct splay_tree_node *left;
  struct splay_tree_node *right;
  struct splay_tree_node *parent;
};

typedef struct splay_tree_node splay_tree_node_t;

struct splay_tree
{
  splay_tree_node_t *root;
};

typedef struct splay_tree splay_tree_t;

typedef splay_tree_t cache_t;

splay_tree_node_t * cache_node_new(syntax_token_t *data);

cache_t *cache_new(void);

void cache_add(splay_tree_t *tree, splay_tree_node_t *node);

void splay_tree_remove(splay_tree_t *t, splay_tree_node_t *n);

syntax_token_t *cache_peek(cache_t *cache);

void cache_pop(cache_t *cache);

#endif/*X_CACHE_H*/
