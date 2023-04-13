/**
 * @file ast.c
 * @author Da'Jour J. Christophe (dajour.christophe@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-04-13
 *
 * @copyright Copyright (c) 2023 Da'Jour J. Christophe. All rights reserved.
 */
#include "ast.h"
#include "expression.h"
#include "syntax-expression-queue.h"
#include "token.h"

/**
 * @brief Print a Syntax Token for development purposes.
 */
void print_token(syntax_token_t *token);

abstract_syntax_tree_t *abstract_syntax_tree_new(void)
{
  abstract_syntax_tree_t *tree = NULL;
  tree = __calloc(1, sizeof(abstract_syntax_tree_t));
  return tree;
}

/*
          0
        /   \
       1     2
      / \   / \
     3   4 5   6
*/

void abstract_syntax_tree_destroy(abstract_syntax_tree_t *tree)
{
  /* NOTE: Perform a BFS to destroy the branches of the tree. */
  __free(tree);
}

static int height(syntax_expression_t *node)
{
  if (node == NULL)
  {
    return 0;
  }
  else
  {
    int lheight = height(node->left);
    int rheight = height(node->right);

    if (lheight > rheight)
    {
      return (lheight + 1);
    }
    else
    {
      return (rheight + 1);
    }
  }
}

static void printCurrentLevel(syntax_expression_t *root, int level)
{
  if (root == NULL)
  {
    return;
  }
  if (level == 1)
  {
    print_token(root->value);
  }
  else if (level > 1)
  {
    printCurrentLevel(root->left, level - 1);
    printCurrentLevel(root->right, level - 1);
  }
}

void printLevelOrder(syntax_expression_t *root)
{
  int h = height(root);
  int i;

  for (i = 1; i <= h; i++)
  {
    printCurrentLevel(root, i);
  }
}

void abstract_syntax_tree_insert(abstract_syntax_tree_t *tree, int kind, syntax_token_t *token)
{
  syntax_expression_t *node = tree->root;

  if (node == NULL)
  {
    tree->root = expression_new(kind, token, NULL, NULL);
    return;
  }

  syntax_expression_queue_t *queue = NULL;
  queue = syntax_expression_queue_new(64);
  syntax_expression_queue_write(queue, node);

  while (node != NULL)
  {
    node = syntax_expression_queue_read(queue);

    if (node->left == NULL)
    {
      node->left = expression_new(kind, token, NULL, NULL);
      break;
    }
    else
    {
      syntax_expression_queue_write(queue, node->left);
    }

    if (node->right == NULL)
    {
      node->right = expression_new(kind, token, NULL, NULL);
    }
    else
    {
      syntax_expression_queue_write(queue, node->right);
    }
  }
}
