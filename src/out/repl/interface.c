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

static char __internal_module_name__[] = "REPL";

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

  x_log(levels.info, "Starting the compile front-end", "After printing the REPL banner, the program is ready to start parsing end-user input");

  symbol_table_t *table = NULL;
  char *line = NULL;

  x_log(levels.info, "Creating the interpreter symbol table", "The interpreter evaluator needs a place to store variable, function, and class data for reuse later in the program");
  table = symbol_table_new();

  x_log(levels.info, "Allocating line buffer for end-user input", "Allocate space on the heap for the line reader to store the end-user input");
  line = __calloc(MAX_LINE, sizeof(char));

  const char fmt[] = "%s";
  const char indicator[] = "> ";

  x_log(levels.info, "Starting the REPL infinite loop", "Loop infinitely so that the end-user can continuously input new data");
  while (true)
  {
    x_log(levels.info, "Printing the REPL end-user indicator", "To avoid appearing as if the program has crashed, display an indication to the end-user to input some data");
    x_log(levels.info, "Scanning a line from the end-user", "Read the end-user input (stdin) from the console for processing");

    printf(fmt, indicator);

    getline(line);

    x_log(levels.info, "Checking if the end-user input is a quit command", "A graceful way for the end-user to exit the program");
    x_log(levels.warn, "Perhaps the performance could be better here", "The program has to revisit the input after having already processed it to store in the line buffer");

    if (strcmp(line, "q") == 0 || strcmp(line, "quit") == 0)
    {
      x_log(levels.debug, "Exiting the interpreter infinite loop in response to the end-user input", "The most semantic approach here is to just stop looping infinitely and let the program leave on its own");
      break;
    }
    else
    {
      x_log(levels.info, "Lexing the end-user input", "For more information about lexing see `lexer.c`");
      syntax_queue_t *queue = compile_line(line);
      x_log(levels.debug, "Lexed the end-user input", "The lexer compile function returns a queue containing generated syntax tokens for the parser to parse");

      x_log(levels.info, "Parsing the end-user input", "For more information about parsing see `parser.c`");
      syntax_expression_t *root = parse(queue);
      x_log(levels.debug, "Parsed the end-user input", "The parser parse function returns an expression tree for processing by the backend");

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

/**
 * @brief
 */
void banner(void)
{
  X_EXPORT_MODULE_NAME_SYMBOL();

  const char fmt[] = "%s\n";
  const char *ptr = banner_message;

  x_log(levels.info, "Printing the REPL banner", "The REPL banner notifies the end-user that the program has started and gives the end-user detailed information about the program");

  printf(fmt, ptr);

  x_log(levels.debug, "Printed the REPL banner", "The ability to print the REPL banner is a good sign as it is an indication the program has initialized.");
}
