

struct syntax_expression
{
  int value;

  struct syntax_expression *left;
  struct syntax_expression *right;
};

typedef struct syntax_expression syntax_expression_t;

struct abstract_syntax_tree
{
  syntax_expression_t root;
};

typedef struct abstract_syntax_tree abstract_syntax_tree_t;

abstract_syntax_tree_t *abstract_syntax_tree_new(void)
{
  abstract_syntax_tree_t *tree = NULL;
  tree = __malloc(sizeof(abstract_syntax_tree_t));
  tree->root = NULL;
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

void printLevelOrder(struct node *root)
{
  int h = height(root);
  int i;

  for (i = 1; i <= h; i++)
  {
    printCurrentLevel(root, i);
  }
}

void printCurrentLevel(struct node *root, int level)
{
  if (root == NULL)
  {
    return;
  }
  if (level == 1)
  {
    printf("%d ", root->data);
  }
  else if (level > 1)
  {
    printCurrentLevel(root->left, level - 1);
    printCurrentLevel(root->right, level - 1);
  }
}

int height(struct node *node)
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

void abstract_syntax_tree_insert(abstract_syntax_tree_t *tree, syntax_expression_t *new_node)
{
  syntax_expression_t *node = tree->root;

  if (node == NULL)
  {
    tree->root = // [new expression node]
    return;
  }

  expression_queue_t *queue = // linked list;

  expression_queue_enqueue(node);

  while (!expression_queue_is_empty(queue))
  {
    node = expression_queue_peek(queue);
    expression_queue_dequeue(queue);

    if (node.left == NULL)
    {
      node.left = // new expression node
      break;
    }
    else
    {
      expression_queue_enqueue(node.left);
    }

    if (node.right == NULL)
    {
      node.right = // new expression node
    }
    else
    {
      expression_queue_enqueue(node.right);
    }
  }
}
