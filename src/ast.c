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

#include <stdio.h>

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

static syntax_expression_t *_abstract_syntax_tree_insert(syntax_expression_t *node, int kind, syntax_token_t *token)
{
  if (node == NULL)
  {
    return expression_new(kind, token, NULL, NULL);
  }

  syntax_expression_queue_t *queue = NULL;
  queue = syntax_expression_queue_new(128);

  if (syntax_expression_queue_write(queue, node) == false)
  {
    fprintf(stderr, "%s\n", "failed to write to syntax expression queue");
    exit(EXIT_FAILURE);
  }

  while (syntax_expression_queue_is_empty(queue) == false)
  {
    node = syntax_expression_queue_read(queue);

    if (node->left != NULL)
    {
      if (syntax_expression_queue_write(queue, node->left) == false)
      {
        fprintf(stderr, "%s\n", "failed to write to syntax expression queue");
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      node->left = expression_new(kind, token, NULL, NULL);
      break;
    }

    if (node->right == NULL)
    {
      if (syntax_expression_queue_write(queue, node->right) == false)
      {
        fprintf(stderr, "%s\n", "failed to write to syntax expression queue");
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      node->right = expression_new(kind, token, NULL, NULL);
      break;
    }
  }

  syntax_expression_queue_destroy(queue);
  return node;
}

void abstract_syntax_tree_insert(abstract_syntax_tree_t *tree, int kind, syntax_token_t *token)
{
  tree->root = _abstract_syntax_tree_insert(tree->root, kind, token);
}
