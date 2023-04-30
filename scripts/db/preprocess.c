/**
 * @file preprocess.c
 * @author Da'Jour J. Christophe (dajour.christophe@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-04-30
 *
 * @copyright Copyright (c) 2023 Da'Jour J. Christophe. All rights reserved.
 */
#include "common.h"
#include "token.h"

#include <sqlite3.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BAD_TOK 0

#define MAX_NAME_COLUMN_LENGTH 64

static char **definitions_buffer = NULL;
static size_t definitions_buffer_size = 0;

static char **returns_buffer = NULL;
static size_t returns_buffer_size = 0;

static void buffer_destroy(void)
{
  for (int i = 0; i < definitions_buffer_size; i++)
  {
    __free(definitions_buffer[i]);
  }
  __free(definitions_buffer);

  for (int i = 0; i < returns_buffer_size; i++)
  {
    __free(returns_buffer[i]);
  }
  __free(returns_buffer);
}

static int definitions_callback(void *NotUsed, int argc, char **argv, char **azColName)
{
  NotUsed = 0;

  for (int i = 0; i < argc; i++)
  {
    definitions_buffer[definitions_buffer_size] = __calloc(MAX_NAME_COLUMN_LENGTH, sizeof(char));
    strcpy(definitions_buffer[definitions_buffer_size++], argv[i] ? argv[i] : "");
  }

  return 0;
}

static int returns_callback(void *NotUsed, int argc, char **argv, char **azColName)
{
  NotUsed = 0;

  for (int i = 0; i < argc; i++)
  {
    returns_buffer[returns_buffer_size] = __calloc(MAX_NAME_COLUMN_LENGTH, sizeof(char));
    strcpy(returns_buffer[returns_buffer_size++], argv[i] ? argv[i] : "");
  }

  return 0;
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

/**
 * @brief
 */
syntax_token_trie_t *global_token_diction;

/**
 * @brief
 */
struct syntax_token_insert
{
  const char str[128];
  syntax_token_t tok;
};

/**
 * @brief
 */
typedef struct syntax_token_insert syntax_token_insert_t;

static int tokens_callback(void *NotUsed, int argc, char **argv, char **azColName)
{
  NotUsed = 0;

  syntax_token_insert_t insertion;
  memset(&insertion, 0, sizeof(syntax_token_insert_t));

  for (int i = 0; i < argc; i++)
  {
    if (strcmp(azColName[i], "repr") == 0)
    {
      strcpy((char *)insertion.str, argv[i] ? argv[i] : "");
    }
    else if (strcmp(azColName[i], "type") == 0)
    {
      memcpy(&insertion.tok.type, argv[i], sizeof(int));
    }
    else if (strcmp(azColName[i], "size") == 0)
    {
      memcpy(&insertion.tok.size, argv[i], sizeof(int));
    }
    else if (strcmp(azColName[i], "data") == 0)
    {
      if (argv[i] != NULL)
      {
        insertion.tok.data = __malloc(128 * sizeof(char));
        memset(insertion.tok.data, 0, 128 * sizeof(char));
        memcpy(insertion.tok.data, argv[i], strlen(argv[i]));
      }
    }
  }

  syntax_token_trie_insert(global_token_diction, insertion.str, &insertion.tok);

  return 0;
}

static int fetch_definitions_from_database(const char *fp)
{
  sqlite3 *db;
  sqlite3_stmt *res;
  char *err_msg = 0;
  int rc;

  rc = sqlite3_open(fp, &db);
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return EXIT_FAILURE;
  }

  definitions_buffer = __calloc(128, sizeof(char *));

  const char query[] = "select name from definition";
  rc = sqlite3_exec(db, query, definitions_callback, 0, &err_msg);
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "Failed to select data\n");
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return EXIT_FAILURE;
  }

  sqlite3_close(db);
  return EXIT_SUCCESS;
}

static int fetch_returns_from_database(const char *fp)
{
  sqlite3 *db;
  sqlite3_stmt *res;
  char *err_msg = 0;
  int rc;

  rc = sqlite3_open(fp, &db);
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return EXIT_FAILURE;
  }

  returns_buffer = __calloc(128, sizeof(char *));

  const char query[] = "select name from return";
  rc = sqlite3_exec(db, query, returns_callback, 0, &err_msg);
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "Failed to select data\n");
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return EXIT_FAILURE;
  }

  sqlite3_close(db);
  return EXIT_SUCCESS;
}

static int fetch_tokens_from_database(const char *fp)
{
  sqlite3 *db;
  sqlite3_stmt *res;
  char *err_msg = 0;
  int rc;

  rc = sqlite3_open(fp, &db);
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return EXIT_FAILURE;
  }

  returns_buffer = __calloc(128, sizeof(char *));

  const char query[] = "select * from token";
  rc = sqlite3_exec(db, query, tokens_callback, 0, &err_msg);
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "Failed to select data\n");
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return EXIT_FAILURE;
  }

  sqlite3_close(db);
  return EXIT_SUCCESS;
}

static char *create_enum(void)
{
  char *output = NULL;
  char tmp[1024];
  int n;

  output = __calloc(4096, sizeof(char));

  strncpy(output, "/* **WARNING** DO NOT EDIT THIS FILE! THIS FILE WAS GENERATED BY THE BUILD. */\n#ifndef X_SYNTAX_FACTS_H\n#define X_SYNTAX_FACTS_H\n\n", 130);

  strncat(output, "enum\n{\n", 8);
  for (int i = 0; i < definitions_buffer_size; i++)
  {
    memset(tmp, 0, 1024 * sizeof(char));
    n = sprintf(tmp, "  %s,\n", definitions_buffer[i]);
    strncat(output, tmp, n);
  }
  strncat(output, "};\n\n", 5);

  strncat(output, "enum\n{\n", 8);
  for (int i = 0; i < returns_buffer_size; i++)
  {
    memset(tmp, 0, 1024 * sizeof(char));
    n = sprintf(tmp, "  %s,\n", returns_buffer[i]);
    strncat(output, tmp, n);
  }
  strncat(output, "};\n\n", 5);

  strncat(output, "#endif/*X_SYNTAX_FACTS_H*/\n", 28);

  buffer_destroy();

  definitions_buffer = NULL;
  definitions_buffer_size = 0;

  returns_buffer = NULL;
  returns_buffer_size = 0;

  return output;
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

int main(void)
{
  global_token_diction = syntax_token_trie_new();

  const char database[] = "tmp/db/syntax.sqlite3";
  // char *output = NULL;
  int rc;

  // rc = fetch_definitions_from_database(database);
  // if (rc != EXIT_SUCCESS)
  // {
  //   fprintf(stderr, "%s\n", "failed to fetch syntax token definitions from the local database");
  //   return EXIT_FAILURE;
  // }
  // rc = fetch_returns_from_database(database);
  // if (rc != EXIT_SUCCESS)
  // {
  //   fprintf(stderr, "%s\n", "failed to fetch syntax token definitions from the local database");
  //   return EXIT_FAILURE;
  // }
  rc = fetch_tokens_from_database(database);
  if (rc != EXIT_SUCCESS)
  {
    fprintf(stderr, "%s\n", "failed to fetch syntax token definitions from the local database");
    return EXIT_FAILURE;
  }

  FILE *fd;
  fd = fopen("trie.data", "wb");
  if (fd == NULL)
  {
    fprintf(stderr, "%s\n", "file couldn't be opened");
    exit(EXIT_FAILURE);
  }

  const size_t n = fwrite(global_token_diction, 1, syntax_token_trie_size(global_token_diction), fd);
  printf("bytes written into file are : %d\n", n);
  fclose(fd);

  // output = create_enum();

  // FILE *fd;
  // fd = fopen("include/facts.h", "wb");
  // if (fd == NULL)
  // {
  //   fprintf(stderr, "%s\n", "file couldn't be opened");
  //   exit(EXIT_FAILURE);
  // }

  // const size_t n = fwrite(output, 1, strlen(output), fd);
  // printf("bytes written into file are : %d\n", n);
  // __free(output);
  // fclose(fd);

  return EXIT_SUCCESS;
}
