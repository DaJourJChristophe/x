
#include "common.h"

#include <stdio.h>
#include <stdlib.h>

struct splay_tree_node
{
  int data;

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

splay_tree_node_t *splay_tree_node_new(int data)
{
  splay_tree_node_t *node = NULL;
  node = __calloc(1, sizeof(splay_tree_node_t));
  node->data = data;
  return node;
}

splay_tree_t *splay_tree_new(void)
{
  splay_tree_t *tree = NULL;
  tree = __calloc(1, sizeof(splay_tree_t));
  return tree;
}

splay_tree_node_t *maximum(splay_tree_t *tree, splay_tree_node_t *node)
{
  while(node->right != NULL)
  {
    node = node->right;
  }
  return node;
}

void left_rotate(splay_tree_t *tree, splay_tree_node_t *x)
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

void right_rotate(splay_tree_t *tree, splay_tree_node_t *x)
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

void splay(splay_tree_t *tree, splay_tree_node_t *n)
{
  while(n->parent != NULL)
  { //node is not root
    if(n->parent == tree->root)
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
      else if(n->parent->right == n && p->parent->right == p)
      { //both are right children
        left_rotate(tree, g);
        left_rotate(tree, p);
      }
      else if(n->parent->right == n && p->parent->left == p)
      {
        left_rotate(tree, p);
        right_rotate(tree, g);
      }
      else if(n->parent->left == n && p->parent->right == p)
      {
        right_rotate(tree, p);
        left_rotate(tree, g);
      }
    }
  }
}

void insert(splay_tree_t *t, splay_tree_node_t *n)
{
  splay_tree_node_t *y = NULL;
  splay_tree_node_t *temp = t->root;

  while (temp != NULL)
  {
    y = temp;

    if(n->data < temp->data)
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

splay_tree_node_t *search(splay_tree_t *t, splay_tree_node_t *n, int x)
{
  if (x == n->data)
  {
    splay(t, n);
    return n;
  }
  else if (x < n->data)
  {
    return search(t, n->left, x);
  }
  else if (x > n->data)
  {
    return search(t, n->right, x);
  }
  else
  {
    return NULL;
  }
}

void delete(splay_tree_t *t, splay_tree_node_t *n)
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
    splay_tree_node_t *m = maximum(left_subtree, left_subtree->root);
    splay(left_subtree, m);
    left_subtree->root->right = right_subtree->root;
    t->root = left_subtree->root;
  }
  else
  {
    t->root = right_subtree->root;
  }
}

void inorder(splay_tree_t *tree, splay_tree_node_t *node)
{
  if (node != NULL)
  {
    inorder(tree, node->left);
    printf("%d\n", node->data);
    inorder(tree, node->right);
  }
}

int main(void)
{
  splay_tree_t *t = splay_tree_new();

  splay_tree_node_t *a, *b, *c, *d, *e, *f, *g, *h, *i, *j, *k, *l, *m;
  a = splay_tree_node_new(10);
  b = splay_tree_node_new(20);
  c = splay_tree_node_new(30);
  d = splay_tree_node_new(100);
  e = splay_tree_node_new(90);
  f = splay_tree_node_new(40);
  g = splay_tree_node_new(50);
  h = splay_tree_node_new(60);
  i = splay_tree_node_new(70);
  j = splay_tree_node_new(80);
  k = splay_tree_node_new(150);
  l = splay_tree_node_new(110);
  m = splay_tree_node_new(120);

  insert(t, a); printf("%d\n", t->root->data);
  insert(t, b); printf("%d\n", t->root->data);
  insert(t, c); printf("%d\n", t->root->data);
  insert(t, d); printf("%d\n", t->root->data);
  insert(t, e); printf("%d\n", t->root->data);
  insert(t, f); printf("%d\n", t->root->data);
  insert(t, g); printf("%d\n", t->root->data);
  insert(t, h); printf("%d\n", t->root->data);
  insert(t, i); printf("%d\n", t->root->data);
  insert(t, j); printf("%d\n", t->root->data);
  insert(t, k); printf("%d\n", t->root->data);
  insert(t, l); printf("%d\n", t->root->data);
  insert(t, m); printf("%d\n", t->root->data);

  printf("\n");

  delete(t, a); printf("%d\n", t->root->data);
  delete(t, m); printf("%d\n", t->root->data);

  // inorder(t, t->root);

  exit(EXIT_SUCCESS);

  return EXIT_SUCCESS;
}
