#include "common.h"

#include <stdbool.h>
#include <string.h>

#define BAD_TOK 0

/**
 * @brief
 */
#define SYNTAX_TOKEN_NODE_CHILD_MAX    ((int)128)

/**
 * @brief Define the syntax token node data structure.
 */
struct syntax_token_node
{
  syntax_token_t tok;
  bool isWordEnd;

  /* An array of syntax token nodes representing the current node's children. */
  struct syntax_token_node **children;
};

/**
 * @brief Define a namespace for the syntax token node structure.
 */
typedef struct syntax_token_node syntax_token_node_t;

/**
 * @brief
 */
syntax_token_node_t *syntax_token_node_new(const syntax_token_t *tok)
{
  syntax_token_node_t *node = NULL;
  node = __malloc(sizeof(syntax_token_node_t));
  node->children = NULL;
  memcpy(&node->tok, tok, sizeof(syntax_token_t));
  return node;
}

/**
 * @brief
 */
void syntax_token_node_destroy(syntax_token_node_t *node)
{
  if (node != NULL && node->children != NULL)
  {
    syntax_token_node_t **children = node->children;

    for (uint64_t i = 0; i < SYNTAX_TOKEN_NODE_CHILD_MAX; i++)
    {
      if (children[i] != NULL)
      {
        syntax_token_node_destroy(children[i]);
      }
    }

    __free(node->children);
  }

  __free(node);
}

/**
 * @brief Define the syntax token node data structure.
 */
struct syntax_token_trie
{
  syntax_token_node_t *root;
};

/**
 * @brief Define a namespace for the syntax token node structure.
 */
typedef struct syntax_token_trie syntax_token_trie_t;

/**
 * @brief
 */
syntax_token_trie_t *syntax_token_trie_new(void)
{
  syntax_token_trie_t *trie = NULL;
  trie = __malloc(sizeof(syntax_token_trie_t));
  trie->root = syntax_token_node_new(&(syntax_token_t){
    .type=0,
    .data=NULL,
    .size=0,
  });
  return trie;
}

/**
 * @brief
 */
void syntax_token_trie_destroy(syntax_token_trie_t *trie)
{
  if (trie != NULL)
  {
    syntax_token_node_destroy(trie->root);
  }
  __free(trie);
}

/**
 * @brief
 */
bool syntax_token_trie_search(syntax_token_trie_t *trie, syntax_token_t *rettok, const char **key)
{
  syntax_token_node_t *node = NULL;

  bool result = false;

  int index;
  int jndex;

  node = trie->root;

  memset(rettok, 0, sizeof(syntax_token_t));
  memcpy(rettok, &(syntax_token_t){.type=BAD_TOK,.data=NULL,.size=0,}, sizeof(syntax_token_t));

  for (; **key; (*key)++)
  {
    index = (int)**key;
    jndex = (int)*(*key + 1);

    if (node->children == NULL || node->children[index] == NULL)
    {
      goto done;
    }

    node = node->children[index];

    if (node->children != NULL && node->children[jndex] != NULL)
    {
      continue;
    }

    *key += 1;

    if (node->isWordEnd)
    {
      goto setret;
    }
    else
    {
      goto done;
    }
  }
setret:
  result = true;
  memset(rettok, 0, sizeof(syntax_token_t));
  memcpy(rettok, &node->tok, sizeof(syntax_token_t));
done:
  return result;
}

/**
 * @brief
 */
void syntax_token_trie_insert(syntax_token_trie_t *trie, const char *key, const syntax_token_t *tok)
{
  syntax_token_node_t *node = NULL;
  int index;

  node = trie->root;

  for (; *key; key++)
  {
    index = (int)*key;

    if (node->children == NULL)
    {
      node->children = __calloc(SYNTAX_TOKEN_NODE_CHILD_MAX, sizeof(syntax_token_node_t *));
    }

    if (node->children[index] == NULL)
    {
      node->children[index] = syntax_token_node_new(tok);
    }

    node = node->children[index];
  }

  node->isWordEnd = true;
}

static size_t _syntax_token_trie_size(syntax_token_node_t *node)
{
  if (node == NULL || node->children == NULL)
  {
    return 0;
  }

  syntax_token_node_t **children = node->children;
  size_t n = 0;

  for (uint64_t i = 0; i < SYNTAX_TOKEN_NODE_CHILD_MAX; i++)
  {
    if (children[i] == NULL)
    {
      continue;
    }

    n += _syntax_token_trie_size(children[i]);
    n++;
  }

  return n;
}

size_t syntax_token_trie_size(syntax_token_trie_t *trie)
{
  if (trie == NULL || trie->root == NULL)
  {
    return 0;
  }
  const size_t n = _syntax_token_trie_size(trie->root);
  return n * sizeof(syntax_token_node_t);
}

void _syntax_token_trie_to_packed(uint8_t *buffer,
  syntax_token_node_t *node, size_t *N)
{
  if (node == NULL)
  {
    return;
  }

  size_t n = 0;
  uint8_t *token_buffer = NULL;
  token_buffer = syntax_token_to_bytes(&node->tok, &n);
  if (token_buffer != NULL)
  {
    memcpy(buffer, token_buffer, n);
    __free(token_buffer);
  }
  *N += n;

  memcpy((buffer + n), &node->isWordEnd, sizeof(bool));
  *N += sizeof(bool);

  syntax_token_node_t **children = node->children;

  if (children == NULL)
  {
    return;
  }

  syntax_token_node_t *child = NULL;

  for (uint64_t i = 0; i < SYNTAX_TOKEN_NODE_CHILD_MAX; i++)
  {
    child = children[i];
    _syntax_token_trie_to_packed((buffer + n + sizeof(bool)), child, N);
  }
}

uint8_t *syntax_token_trie_to_packed(syntax_token_trie_t *trie, size_t *n)
{
  syntax_token_node_t *node = trie->root;
  if (node == NULL)
  {
    *n = 0;
    return NULL;
  }

  uint8_t *buffer = NULL;
  buffer = __calloc((1 << 20), sizeof(uint8_t));
  _syntax_token_trie_to_packed(buffer, node, n);
  return buffer;
}
