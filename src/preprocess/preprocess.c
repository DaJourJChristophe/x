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
#include "preprocess.h"
#include "token.h"

#include <sqlite3.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief
 */
syntax_token_trie_t *global_token_diction;

int main(void)
{
  global_token_diction = syntax_token_trie_new();

  const char database[] = "tmp/db/syntax.sqlite3";
  char *output = NULL;
  int rc;

  rc = fetch_definitions_from_database(database);
  if (rc != EXIT_SUCCESS)
  {
    fprintf(stderr, "%s\n", "failed to fetch syntax token definitions from the local database");
    return EXIT_FAILURE;
  }
  rc = fetch_returns_from_database(database);
  if (rc != EXIT_SUCCESS)
  {
    fprintf(stderr, "%s\n", "failed to fetch syntax token definitions from the local database");
    return EXIT_FAILURE;
  }

  output = create_enum();

  FILE *fd;
  fd = fopen("include/facts.h", "wb");
  if (fd == NULL)
  {
    fprintf(stderr, "%s\n", "file couldn't be opened");
    exit(EXIT_FAILURE);
  }

  size_t n;

  n = fwrite(output, 1, strlen(output), fd);
  printf("bytes written into file are : %lu\n", n);
  __free(output);
  fclose(fd);
  fd = NULL;

  rc = fetch_tokens_from_database(database);
  if (rc != EXIT_SUCCESS)
  {
    fprintf(stderr, "%s\n", "failed to fetch syntax token definitions from the local database");
    return EXIT_FAILURE;
  }

  fd = fopen("trie.data", "wb");
  if (fd == NULL)
  {
    fprintf(stderr, "%s\n", "file couldn't be opened");
    exit(EXIT_FAILURE);
  }

  size_t data_size = 0;
  uint8_t *data = syntax_token_trie_to_packed(global_token_diction, &data_size);

  n = fwrite(data, 1, data_size, fd);
  printf("bytes written into file are : %lu\n", n);
  __free(data);
  fclose(fd);
  data = NULL;
  fd = NULL;

  exit(EXIT_SUCCESS);

  return EXIT_SUCCESS;
}
