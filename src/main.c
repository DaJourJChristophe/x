#include "lexer.h"
#include "parse.h"
#include "queue.h"

#include <stdlib.h>

int main(void)
{
  const char filepath[] = "./example/main.x";

  syntax_queue_t *queue = compile(filepath);

  parse(queue);

  syntax_queue_destroy(queue);

  /* Standard int to hold length of match */
  // int match_length;

  /* Standard null-terminated C-string to search: */
  // const char string_to_search[] = "abc123";

  /* Compile a simple regular expression using character classes, meta-char and greedy + non-greedy quantifiers: */
  // re_t pattern = re_compile("[a-z]*[0-9]*");

  /* Check if the regex matches the text: */
  // int match_idx = re_match_compiled(pattern, string_to_search, &match_length);
  // if (match_idx != (-1))
  // {
  //   printf("match at idx %i, %i chars long.\n", match_idx, match_length);
  // }
  // else
  // {
  //   printf("%s\n", "no match");
  // }

  return EXIT_SUCCESS;
}
