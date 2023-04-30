#include "common.h"
#include "token.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BAD_TOK 0

/**
 * @brief Open a file on the disk, determine it's size, and then
 *        read the entire file from the disk into memory.
 */
int readfile(void *buffer, const char *file_path)
{
  char const mode[] = "rb";
  FILE *fd = NULL;
  long fsize;
  int bytes = 0;

  fd = fopen(file_path, mode);
  if (fd == NULL)
  {
    fprintf(stderr, "fopen() failed to open a file on the disk\n");
    return (-1);
  }

  fseek(fd, 0, SEEK_END);
  if (ferror(fd))
  {
    fprintf(stderr, "fseek() failed\n");

    int r = fclose(fd);
    if (r == EOF)
    {
      fprintf(stderr, "cannot close file handler\n");
    }

    fd = NULL;
    return (-1);
  }

  fsize = ftell(fd);
  if (fsize == (-1))
  {
    fprintf(stderr, "fseek() failed\n");

    int r = fclose(fd);
    if (r == EOF)
    {
      fprintf(stderr, "cannot close file handler\n");
    }

    fd = NULL;
    return (-1);
  }

  rewind(fd);

  bytes = fread(buffer, 1, fsize, fd);
  if (bytes != fsize)
  {
    fprintf(stderr, "fread() failed to read all bytes from file\n");

    int r = fclose(fd);
    if (r == EOF)
    {
      fprintf(stderr, "cannot close file handler\n");
    }

    fd = NULL;
    return -1;
  }

  if (ferror(fd))
  {
    fprintf(stderr, "fread() failed\n");

    int r = fclose(fd);
    if (r == EOF)
    {
      fprintf(stderr, "cannot close file handler\n");
    }

    fd = NULL;
    return (-1);
  }

  int r = fclose(fd);
  if (r == EOF)
  {
    fprintf(stderr, "cannot close file handler\n");
  }

  fd = NULL;
  return bytes;
}

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

int main(void)
{
  syntax_token_trie_t trie;
  char buffer[(1 << 20)];
  int n;

  memset(&trie, 0, sizeof(syntax_token_trie_t));
  memset(buffer, 0, (1 << 20) * sizeof(char));

  n = readfile(buffer, "trie.data");
  if (n < 0)
  {
    fprintf(stderr, "%s\n", "Failed to read the data file from the disk.");
    exit(EXIT_FAILURE);
  }
  else if (n >= (1 << 20))
  {
    fprintf(stderr, "%s\n", "Attempting to read too much data into the data buffer");
    exit(EXIT_FAILURE);
  }

  memcpy(&trie, buffer, n);

  if (trie.root == NULL)
  {
    fprintf(stderr, "%s\n", "Trie root is null");
    exit(EXIT_FAILURE);
  }
  else if (trie.root->children == NULL)
  {
    fprintf(stderr, "%s\n", "Trie root.children is null");
    exit(EXIT_FAILURE);
  }

  const char data[] = "||";
  const char *original = data;
  const char **key = &original;
  syntax_token_t rettok;
  memset(&rettok, 0, sizeof(syntax_token_t));

  printf("%u\n", syntax_token_trie_search(&trie, &rettok, key));

  return EXIT_SUCCESS;
}

