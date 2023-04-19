#include "common.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE    128

#define LINE_BREAK   '\n'
#define TERMINATE    '\0'

static void getline(char *buf)
{
  char *ptr = buf;

  while (true)
  {
    *ptr = fgetc(stdin);

    if (*ptr == EOF || *ptr == LINE_BREAK)
    {
      break;
    }

    ptr++;
  }

  *ptr = TERMINATE;
}

void getexpr(void)
{
  char *line = NULL;

  line = __calloc(MAX_LINE, sizeof(char));

  while (true)
  {
    printf("%s", "> ");

    getline(line);

    if (strcmp(line, "q") == 0 || strcmp(line, "quit") == 0)
    {
      break;
    }
    else
    {
      printf("%s\n", line);
    }
  }

  __free(line);
}

static const char banner_message[] = "X 1.0.0 (main, Mar 12 2023, 07:10:41) [GCC 10.2.1 20210110] on linux\nType \"help\", \"copyright\", \"credits\" or \"license\" for more information.";

void banner(void)
{
  printf("%s\n", banner_message);
}

int main(void)
{
  banner();
  getexpr();

  return EXIT_SUCCESS;
}
