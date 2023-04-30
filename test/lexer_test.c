#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include "cmocka.h"
#include "facts.h"
#include "lexer.h"
#include "syntax-queue.h"
#include "token.h"
#include "utils.h"

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
    .input="\n",
    .expected={
      {.type=EOL_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="\t",
    .expected={
      {.type=TAB_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input=" ",
    .expected={
      {.type=SPACE_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input=";",
    .expected={
      {.type=EOE_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="<",
    .expected={
      {.type=LEFT_CARET_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="{",
    .expected={
      {.type=OPEN_CURLY_BRACKET_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="(",
    .expected={
      {.type=OPEN_PARENTHESIS_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="[",
    .expected={
      {.type=OPEN_SQUARE_BRACKET_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="}",
    .expected={
      {.type=CLOSE_CURLY_BRACKET_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input=")",
    .expected={
      {.type=CLOSE_PARENTHESIS_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="]",
    .expected={
      {.type=CLOSE_SQUARE_BRACKET_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input=">",
    .expected={
      {.type=RIGHT_CARET_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="@",
    .expected={
      {.type=ANNOTATION_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input=",",
    .expected={
      {.type=COMMA_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input=":",
    .expected={
      {.type=COLON_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="\%",
    .expected={
      {.type=MODULUS_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="=",
    .expected={
      {.type=EQUAL_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input=":",
    .expected={
      {.type=COLON_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="*",
    .expected={
      {.type=STAR_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="/",
    .expected={
      {.type=SLASH_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="+",
    .expected={
      {.type=PLUS_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="-",
    .expected={
      {.type=HYPHEN_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="**",
    .expected={
      {.type=EXPONENTIAL_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="#",
    .expected={
      {.type=REMAINDER_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="&",
    .expected={
      {.type=BITWISE_AND_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="|",
    .expected={
      {.type=BITWISE_OR_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="^",
    .expected={
      {.type=BITWISE_XOR_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="~",
    .expected={
      {.type=BITWISE_TERNARY_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="<<",
    .expected={
      {.type=BITWISE_SHFT_LEFT_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input=">>",
    .expected={
      {.type=BITWISE_SHFT_RIGHT_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="&&",
    .expected={
      {.type=COND_AND_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="||",
    .expected={
      {.type=COND_OR_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="--",
    .expected={
      {.type=DECREMENT_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="++",
    .expected={
      {.type=INCREMENT_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="==",
    .expected={
      {.type=COND_EQUALS_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="abstract",
    .expected={
      {.type=ABSTRACT_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="boolean",
    .expected={
      {.type=BOOLEAN_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="break",
    .expected={
      {.type=BREAK_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="case",
    .expected={
      {.type=CASE_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="class",
    .expected={
      {.type=CLASS_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="const",
    .expected={
      {.type=CONST_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="default",
    .expected={
      {.type=DEFAULT_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="double",
    .expected={
      {.type=DOUBLE_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="export",
    .expected={
      {.type=EXPORT_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="false",
    .expected={
      {.type=FALSE_TOK,.data=&(int){0},.size=sizeof(int),},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="float",
    .expected={
      {.type=FLOAT_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="for",
    .expected={
      {.type=FOR_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="if",
    .expected={
      {.type=IF_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="immutable",
    .expected={
      {.type=IMMUTABLE_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="import",
    .expected={
      {.type=IMPORT_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="int",
    .expected={
      {.type=INTEGER_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="is",
    .expected={
      {.type=IS_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="matrix",
    .expected={
      {.type=MATRIX_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="null",
    .expected={
      {.type=NIL_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="object",
    .expected={
      {.type=OBJECT_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="package",
    .expected={
      {.type=PACKAGE_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="print",
    .expected={
      {.type=PRINT_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="private",
    .expected={
      {.type=PRIVATE_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="protected",
    .expected={
      {.type=PROTECTED_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="public",
    .expected={
      {.type=PUBLIC_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="return",
    .expected={
      {.type=RETURN_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="scalar",
    .expected={
      {.type=SCALAR_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="set",
    .expected={
      {.type=SET_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="static",
    .expected={
      {.type=STATIC_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="string",
    .expected={
      {.type=STRING_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="switch",
    .expected={
      {.type=SWITCH_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="true",
    .expected={
      {.type=TRUE_TOK,.data=&(int){1},.size=sizeof(int),},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="unless",
    .expected={
      {.type=UNLESS_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="vector",
    .expected={
      {.type=VECTOR_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="void",
    .expected={
      {.type=VOID_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="while",
    .expected={
      {.type=WHILE_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
    },
  },{
    .input="yield",
    .expected={
      {.type=YIELD_TOK,.data=NULL,.size=0,},
      {.type=EOF_TOK,.data=NULL,.size=0,},
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
      // printf("from actual: ");
      // print_token(token);

      // printf("from expected: ");
      // print_token((syntax_token_t *)&data[i].expected[j]);

      const bool result = compare_token(token, &data[i].expected[j++]);
      if (result == false)
      {
        printf("%s %s\n", "caught a fail by", data[i].input);
      }
      assert_true(result);
    }

    // printf("\n");

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
