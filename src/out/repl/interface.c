#include "common.h"
#include "diagnostic.h"
#include "lexer.h"
#include "parse.h"
#include "syntax-queue.h"
#include "eval.h"
#include "symbol-table.h"
#include "log/log.h"
#include "internal/module.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char __internal_module_name__[] = "REPL";

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
  X_EXPORT_MODULE_NAME_SYMBOL();

  x_log(levels.info, "Starting the compile front-end", "");

  symbol_table_t *table = NULL;
  char *line = NULL;

  x_log(levels.info, "Creating the interpreter symbol table", "");
  table = symbol_table_new();

  x_log(levels.info, "Allocating line buffer for end-user input", "");
  line = __calloc(MAX_LINE, sizeof(char));

  x_log(levels.info, "Starting the REPL infinite loop", "");
  while (true)
  {
    x_log(levels.info, "Printing the REPL end-user indicator", "");
    x_log(levels.info, "Scanning a line from the end-user", "");

    printf("%s", "> ");

    getline(line);

    x_log(levels.info, "Checking if the end-user input is a quit command", "");
    if (strcmp(line, "q") == 0 || strcmp(line, "quit") == 0)
    {
      x_log(levels.debug, "Exiting the interpreter infinite loop in response to the end-user input", "");
      break;
    }
    else
    {
      x_log(levels.info, "Lexing the end-user input", "");
      syntax_queue_t *queue = compile_line(line);
      x_log(levels.debug, "Lexed the end-user input", "");

      x_log(levels.info, "Parsing the end-user input", "");
      syntax_expression_t *root = parse(queue);
      x_log(levels.debug, "Parsed the end-user input", "");

      x_log(levels.info, "Destroying the syntax token queue", "");
      syntax_queue_destroy(queue);
      x_log(levels.debug, "Destroyed the syntax token queue", "");

      x_log(levels.info, "Displaying the end-user diagnostics", "");
      diagnostic_show(diagnostics);
      x_log(levels.debug, "Displayed the end-user diagnostics", "");

      x_log(levels.info, "Destroying the end-user diagnostics", "");
      diagnostic_destroy(diagnostics);
      x_log(levels.debug, "Destroyed the end-user diagnostics", "");

      x_log(levels.info, "Checking if the parser returned an expression tree", "");
      if (root != NULL)
      {
        x_log(levels.info, "Evaluating the end-user expression tree", "");
        eval(root, table);
        x_log(levels.debug, "Evaluated the end-user expression tree", "");
      }
    }
  }
  x_log(levels.debug, "Exited the REPL infinite loop", "");

  x_log(levels.info, "Destroying the end-user input", "");
  __free(line);
  x_log(levels.debug, "Destroyed the end-user input", "");

  x_log(levels.info, "Destroying the interpreter symbol table", "");
  symbol_table_destroy(table);
  x_log(levels.debug, "Destroyed the interpreter symbol table", "");
}

static const char banner_message[] = "X 1.0.0 (main, Mar 12 2023, 07:10:41) [GCC 10.2.1 20210110] on linux\nType \"help\", \"copyright\", \"credits\" or \"license\" for more information.";

void banner(void)
{
  X_EXPORT_MODULE_NAME_SYMBOL();

  x_log(levels.info, "Printing the REPL banner", "");
  printf("%s\n", banner_message);
  x_log(levels.debug, "Printed the REPL banner", "");
}
