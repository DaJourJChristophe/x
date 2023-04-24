#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include "cmocka.h"
#include "facts.h"
#include "lexer.h"
#include "syntax-queue.h"
#include "token.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static bool compare_token(const syntax_token_t *a, const syntax_token_t *b)
{
  if (a == NULL || b == NULL)
  {
    return false;
  }
  if (a->type != b->type)
  {
    return false;
  }
  if (a->size != b->size)
  {
    return false;
  }
  if (0 != memcmp(a->data, b->data, a->size || b->size))
  {
    return false;
  }
  return true;
}

struct test_data
{
  const char input[128];
  const syntax_token_t expected[10];
};

static void compile_line_test(void unused **state)
{
  struct test_data data[] = {{
    .input="abstract",
    .expected={
      {.type=ABSTRACT,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="boolean",
    .expected={
      {.type=BOOLEAN,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="break",
    .expected={
      {.type=BREAK,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="case",
    .expected={
      {.type=CASE,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="class",
    .expected={
      {.type=CLASS,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="const",
    .expected={
      {.type=CONST,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="default",
    .expected={
      {.type=DEFAULT,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="double",
    .expected={
      {.type=DOUBLE,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="export",
    .expected={
      {.type=EXPORT,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="false",
    .expected={
      {.type=FALSE,.data=&(int){0},.size=sizeof(int),},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="float",
    .expected={
      {.type=FLOAT,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="for",
    .expected={
      {.type=FOR,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="if",
    .expected={
      {.type=IF,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="immutable",
    .expected={
      {.type=IMMUTABLE,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="import",
    .expected={
      {.type=IMPORT,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="int",
    .expected={
      {.type=INTEGER,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="is",
    .expected={
      {.type=IS,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="matrix",
    .expected={
      {.type=MATRIX,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="null",
    .expected={
      {.type=NIL,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="object",
    .expected={
      {.type=OBJECT,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="package",
    .expected={
      {.type=PACKAGE,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="print",
    .expected={
      {.type=PRINT,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="private",
    .expected={
      {.type=PRIVATE,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="protected",
    .expected={
      {.type=PROTECTED,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="public",
    .expected={
      {.type=PUBLIC,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="return",
    .expected={
      {.type=RETURN,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="scalar",
    .expected={
      {.type=SCALAR,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="set",
    .expected={
      {.type=SET,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="static",
    .expected={
      {.type=STATIC,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="string",
    .expected={
      {.type=STRING,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="switch",
    .expected={
      {.type=SWITCH,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="true",
    .expected={
      {.type=TRUE,.data=&(int){1},.size=sizeof(int),},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="unless",
    .expected={
      {.type=UNLESS,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="vector",
    .expected={
      {.type=VECTOR,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="void",
    .expected={
      {.type=VOID,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="while",
    .expected={
      {.type=WHILE,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  },{
    .input="yield",
    .expected={
      {.type=YIELD,.data=NULL,.size=0,},
      {.type=LEXER_EOF,.data=NULL,.size=0,},
    },
  }};

  const size_t n = sizeof(data) / sizeof(data[0]);

  syntax_queue_t *tokens = NULL;
  syntax_token_t *token = NULL;

  for (uint64_t j, i = 0; i < n; i++)
  {
    tokens = compile_line(data[i].input);
    j = 0;

    while ((token = syntax_queue_read(tokens)) != NULL)
    {
      assert_true(compare_token(token, &data[i].expected[j++]));
    }

    syntax_queue_destroy(tokens);
  }
}

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(compile_line_test),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
