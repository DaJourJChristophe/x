#include "common.h"
#include "token.h"
#include "cache.h"

splay_tree_node_t *splay_tree_node_new(syntax_token_t *data)
{
  splay_tree_node_t *node = NULL;
  node = __calloc(1, sizeof(splay_tree_node_t));
  node->data = data;
  return node;
}

inline splay_tree_node_t * always_inline cache_node_new(syntax_token_t *data)
{
  return splay_tree_node_new(data);
}

splay_tree_t *splay_tree_new(void)
{
  splay_tree_t *tree = NULL;
  tree = __calloc(1, sizeof(splay_tree_t));
  return tree;
}

inline cache_t * always_inline cache_new(void)
{
  return splay_tree_new();
}

static splay_tree_node_t *maximum(splay_tree_node_t *node)
{
  while (node->right != NULL)
  {
    node = node->right;
  }
  return node;
}

static void left_rotate(splay_tree_t *tree, splay_tree_node_t *x)
{
  splay_tree_node_t *y = x->right;
  x->right = y->left;
  if(y->left != NULL)
  {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == NULL)
  { //x is root
    tree->root = y;
  }
  else if(x == x->parent->left)
  { //x is left child
    x->parent->left = y;
  }
  else { //x is right child
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

static void right_rotate(splay_tree_t *tree, splay_tree_node_t *x)
{
  splay_tree_node_t *y = x->left;
  x->left = y->right;
  if(y->right != NULL)
  {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == NULL)
  { //x is root
    tree->root = y;
  }
  else if(x == x->parent->right)
  { //x is left child
    x->parent->right = y;
  }
  else { //x is right child
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}

static void splay(splay_tree_t *tree, splay_tree_node_t *n)
{
  while (n->parent != NULL)
  { //node is not root
    if (n->parent == tree->root)
    { //node is child of root, one rotation
      if(n == n->parent->left)
      {
        right_rotate(tree, n->parent);
      }
      else
      {
        left_rotate(tree, n->parent);
      }
    }
    else
    {
      splay_tree_node_t *p = n->parent;
      splay_tree_node_t *g = p->parent; //grandparent

      if (n->parent->left == n && p->parent->left == p)
      { //both are left children
        right_rotate(tree, g);
        right_rotate(tree, p);
      }
      else if (n->parent->right == n && p->parent->right == p)
      { //both are right children
        left_rotate(tree, g);
        left_rotate(tree, p);
      }
      else if (n->parent->right == n && p->parent->left == p)
      {
        left_rotate(tree, p);
        right_rotate(tree, g);
      }
      else if (n->parent->left == n && p->parent->right == p)
      {
        right_rotate(tree, p);
        left_rotate(tree, g);
      }
    }
  }
}

void splay_tree_insert(splay_tree_t *t, splay_tree_node_t *n)
{
  splay_tree_node_t *y = NULL;
  splay_tree_node_t *temp = t->root;

  while (temp != NULL)
  {
    y = temp;

    if (n->data < temp->data)
    {
      temp = temp->left;
    }
    else
    {
      temp = temp->right;
    }
  }

  n->parent = y;

  if (y == NULL)
  {//newly added node is root
    t->root = n;
  }
  else if(n->data < y->data)
  {
    y->left = n;
  }
  else
  {
    y->right = n;
  }

  splay(t, n);
}

void cache_add(splay_tree_t *tree, splay_tree_node_t *node)
{
  splay_tree_insert(tree, node);
}

void splay_tree_remove(splay_tree_t *t, splay_tree_node_t *n)
{
  splay(t, n);

  splay_tree_t *left_subtree = splay_tree_new();
  left_subtree->root = t->root->left;
  if (left_subtree->root != NULL)
  {
    left_subtree->root->parent = NULL;
  }

  splay_tree_t *right_subtree = splay_tree_new();
  right_subtree->root = t->root->right;
  if (right_subtree->root != NULL)
  {
    right_subtree->root->parent = NULL;
  }

  __free(n);

  if (left_subtree->root != NULL)
  {
    splay_tree_node_t *m = maximum(left_subtree->root);
    splay(left_subtree, m);
    left_subtree->root->right = right_subtree->root;
    t->root = left_subtree->root;
  }
  else
  {
    t->root = right_subtree->root;
  }
}

void cache_remove(splay_tree_t *tree, splay_tree_node_t *node)
{
  splay_tree_remove(tree, node);
}

// int main(void)
// {
//   splay_tree_t *t = splay_tree_new();

//   syntax_token_t token1 = {
//     .type=1,
//     .data=NULL,
//   };

//   syntax_token_t token2 = {
//     .type=2,
//     .data=NULL,
//   };

//   splay_tree_node_t *a;

//   a = splay_tree_node_new(&token1);
//   splay_tree_insert(t, a); printf("%d\n", t->root->data->type);

//   a = splay_tree_node_new(&token2);
//   splay_tree_insert(t, a); printf("%d\n", t->root->data->type);

//   exit(EXIT_SUCCESS);

//   return EXIT_SUCCESS;
// }

syntax_token_t *cache_peek(cache_t *cache)
{
  return cache->root->data;
}

void cache_pop(cache_t *cache)
{
  cache_remove(cache, cache->root);
}
