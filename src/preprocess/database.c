#include "common.h"

#include <sqlite3.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NAME_COLUMN_LENGTH 64

static char **definitions_buffer = NULL;
static size_t definitions_buffer_size = 0;

static char **returns_buffer = NULL;
static size_t returns_buffer_size = 0;

/**
 * @brief
 */
static int definitions_callback(void unused *NotUsed, int argc, char **argv, char unused **azColName)
{
  NotUsed = 0;

  for (int i = 0; i < argc; i++)
  {
    definitions_buffer[definitions_buffer_size] = __calloc(MAX_NAME_COLUMN_LENGTH, sizeof(char));
    strcpy(definitions_buffer[definitions_buffer_size++], argv[i] ? argv[i] : "");
  }

  return 0;
}

/**
 * @brief
 */
static int returns_callback(void unused *NotUsed, int argc, char **argv, char unused **azColName)
{
  NotUsed = 0;

  for (int i = 0; i < argc; i++)
  {
    returns_buffer[returns_buffer_size] = __calloc(MAX_NAME_COLUMN_LENGTH, sizeof(char));
    strcpy(returns_buffer[returns_buffer_size++], argv[i] ? argv[i] : "");
  }

  return 0;
}

static int tokens_callback(void unused *NotUsed, int argc, char **argv, char unused **azColName)
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
      insertion.tok.type = strtol(argv[i], NULL, 10);
    }
    else if (strcmp(azColName[i], "size") == 0)
    {
      insertion.tok.size = strtol(argv[i], NULL, 10);
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

/**
 * @brief
 */
int fetch_definitions_from_database(const char *fp)
{
  sqlite3 *db;
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

/**
 * @brief
 */
int fetch_returns_from_database(const char *fp)
{
  sqlite3 *db;
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

/**
 * @brief
 */
int fetch_tokens_from_database(const char *fp)
{
  sqlite3 *db;
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
