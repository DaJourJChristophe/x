#include "lexer.h"
#include "parse.h"
#include "syntax-queue.h"

#include <stdlib.h>

int main(void)
{
  const char filepath[] = "./example/main.x";

  syntax_queue_t *queue = compile(filepath);

  parse(queue);

  syntax_queue_destroy(queue);

  exit(EXIT_SUCCESS);

  return EXIT_SUCCESS;
}
