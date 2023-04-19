#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
  BINARY_EXPRESSION,
  NUMBER_LITERAL,
  ADD_EXPRESSION,
  MUL_EXPRESSION,
  NUMBER_ONE_LITERAL,
  NUMBER_TWO_LITERAL,
  NUMBER_THREE_LITERAL,
  NUMBER_FOUR_LITERAL,
  NUMBER_FIVE_LITERAL,
};

struct tree_node
{
  int kind;

  struct tree_node *left;
  struct tree_node *right;
  struct tree_node *parent;
};

typedef struct tree_node tree_node_t;

tree_node_t *tree_node_new(int kind)
{
  tree_node_t *node = NULL;
  node = malloc(sizeof(tree_node_t));
  if (node == NULL)
  {
    fprintf(stderr, "%s\n", "memory error");
    exit(EXIT_FAILURE);
  }
  node->kind   = kind;
  node->left   = NULL;
  node->right  = NULL;
  node->parent = NULL;
  return node;
}

void tree_node_destroy(tree_node_t *node)
{
  if (node != NULL)
  {
    free(node);
    node = NULL;
  }
}

struct tree
{
  tree_node_t *root; /* first node */
  tree_node_t *curr; /* last inserted position */
};

typedef struct tree tree_t;

tree_t *tree_new(void)
{
  tree_t *tree = NULL;
  tree = malloc(sizeof(tree_t));
  if (tree == NULL)
  {
    fprintf(stderr, "%s\n", "memory error");
    exit(EXIT_FAILURE);
  }
  tree->root = NULL;
  return tree;
}

void tree_destroy(tree_t *tree)
{
  if (tree != NULL)
  {
    free(tree);
    tree = NULL;
  }
}

void _tree_insert_number_literal(tree_t *tree, int kind)
{
  if (tree->root == NULL)
  {
    tree->root = tree_node_new(kind);
    tree->curr = tree->root;
    return;
  }
  if (tree->curr)
  {
    if (tree->curr->parent)
    {
      switch (tree->curr->parent->kind)
      {
        case BINARY_EXPRESSION:
        case ADD_EXPRESSION:
        case MUL_EXPRESSION:
          if (tree->curr->parent->right == NULL)
          {
            tree->curr->parent->right = tree_node_new(kind);
            tree->curr->parent->right->parent = tree->curr;
          }
          break;

        default:
          tree->curr->left = tree_node_new(kind);
          tree->curr->left->parent = tree->curr;
          tree->curr = tree->curr->left;
      }
    }
    else
    {
      tree->curr->left = tree_node_new(kind);
      tree->curr->left->parent = tree->curr;
      tree->curr = tree->curr->left;
    }
  }
}

void _tree_insert_binary_expression(tree_t *tree, int kind)
{
  if (tree->root == NULL)
  {
    tree->root = tree_node_new(kind);
    tree->curr = tree->root;
    return;
  }

  tree->curr->left = tree_node_new(kind);

  if (tree->curr->right && tree->curr->parent)
  {

  }
  else if (tree->curr->right && !tree->curr->parent)
  {

  }
  else if (!tree->curr->right &&  tree->curr->parent &&
                                  tree->curr->parent->parent)
  {
    tree->curr->left->right = tree->curr->parent->left;
    tree->curr->left->left = tree->curr->parent->parent->left;
    tree->curr->left->parent = tree->curr->parent->parent;

    tree->curr->parent->parent->left = tree->curr->left;
    tree->curr = tree->curr->left;

    tree->curr->right->parent = tree->curr;
    tree->curr->left->parent = tree->curr;

    tree->curr->right->left = NULL;
    tree->curr->left->left = NULL;

    tree->curr = tree->curr->left;
  }
  else if (!tree->curr->right && tree->curr->parent)
  {
    tree->curr->left->right = tree->curr;
    tree->curr->left->left = tree->curr->parent;
    tree->curr->left->parent = tree->curr->parent->parent;

    tree->root = tree->curr->left;
    tree->curr = tree->curr->left;

    tree->curr->right->parent = tree->curr;
    tree->curr->left->parent = tree->curr;

    tree->curr->right->left = NULL;
    tree->curr->left->left = NULL;

    tree->curr = tree->curr->left;
  }
  else if (!tree->curr->right && !tree->curr->parent)
  {
    tree->curr->left->left = tree->curr;
    tree->curr->left->parent = tree->curr->parent;

    tree->root = tree->curr->left;
    tree->curr = tree->curr->left;

    tree->curr->left->parent = tree->curr;

    tree->curr->left->left = NULL;

    tree->curr = tree->curr->left;
  }
}

void tree_traverse(tree_t *tree);

int main(void)
{
  tree_t *tree = tree_new();

  //infix example
  // tree_insert(tree, NUMBER_ONE_LITERAL);
  // tree_insert(tree, ADD_EXPRESSION);
  // tree_insert(tree, NUMBER_TWO_LITERAL);
  // tree_insert(tree, ADD_EXPRESSION);
  // tree_insert(tree, NUMBER_THREE_LITERAL);

  // infix:   1 + 2 + 3
  // postfix: 1 2 3 + +

  //postfix example
  _tree_insert_number_literal(tree, NUMBER_ONE_LITERAL);
  _tree_insert_binary_expression(tree, ADD_EXPRESSION);
  _tree_insert_number_literal(tree, NUMBER_TWO_LITERAL);
  // _tree_insert_binary_expression(tree, MUL_EXPRESSION);

  printf("%p %p\n", (void *)tree->root->left,
                    (void *)tree->root->right);

  tree_traverse(tree);

  exit(EXIT_SUCCESS);
  tree_destroy(tree);
  return EXIT_SUCCESS;
}




#define MAXBUF (1 << 20)
#define MINBUF (1 << 8)

void _tree_traverse(tree_node_t *node, char *buffer, char *padding, char *pointer)
{
  if (node == NULL)
  {
    return;
  }

  strcat(buffer, padding);
  strcat(buffer, pointer);

  switch (node->kind)
  {
    case NUMBER_LITERAL:
      strcat(buffer, "\033[0;45mNUMBER LITERAL\033[0m");
      break;

    case NUMBER_ONE_LITERAL:
      strcat(buffer, "\033[0;45mONE LITERAL\033[0m");
      break;

    case NUMBER_TWO_LITERAL:
      strcat(buffer, "\033[0;45mTWO LITERAL\033[0m");
      break;

    case NUMBER_THREE_LITERAL:
      strcat(buffer, "\033[0;45mTHREE LITERAL\033[0m");
      break;

    case NUMBER_FOUR_LITERAL:
      strcat(buffer, "\033[0;45mFOUR LITERAL\033[0m");
      break;

    case NUMBER_FIVE_LITERAL:
      strcat(buffer, "\033[0;45mFIVE LITERAL\033[0m");
      break;

    case BINARY_EXPRESSION:
      strcat(buffer, "\033[0;44mBINARY EXPRESSION\033[0m");
      break;

    case ADD_EXPRESSION:
      strcat(buffer, "\033[0;44mADD EXPRESSION\033[0m");
      break;

    case MUL_EXPRESSION:
      strcat(buffer, "\033[0;44mMUL EXPRESSION\033[0m");
      break;

    default:
      fprintf(stderr, "%s\n", "unknown node kind");
      exit(EXIT_FAILURE);
  }

  strcat(buffer, "\n");

  char padding_for_both[MINBUF];
  memset(padding_for_both, 0, MINBUF * sizeof(char));
  strcat(padding_for_both, padding);
  strcat(padding_for_both, "│  ");

  char pointer_for_right[MINBUF];
  memset(pointer_for_right, 0, MINBUF * sizeof(char));
  strcat(pointer_for_right, "└──");

  char pointer_for_left[MINBUF];
  memset(pointer_for_left, 0, MINBUF * sizeof(char));
  strcat(pointer_for_left, ((node->right != NULL) ? "├──" : "└──"));

  _tree_traverse(node->left,  buffer, padding_for_both, pointer_for_left);
  _tree_traverse(node->right, buffer, padding_for_both, pointer_for_right);
}

void tree_traverse(tree_t *tree)
{
  char buffer[MAXBUF];
  char padding[MINBUF];
  char pointer[MINBUF];

  memset(buffer,  0, MAXBUF * sizeof(char));
  memset(padding, 0, MINBUF * sizeof(char));
  memset(pointer, 0, MINBUF * sizeof(char));

  _tree_traverse(tree->root, buffer, padding, pointer);

  printf("\033[0;37m%s\033[0m", buffer);
}




