#include "common.h"
#include "error.h"
#include "token.h"

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Define a collection of character tokens.
 */
enum
{
  /**
   * @brief Terminating Tokens.
   */
  BAD_TOK = 0,
  EOE_TOK,
  EOF_TOK,
  EOL_TOK,

  /**
   * @brief General Purpose Tokens.
   */
  APOSTROPHE_TOK,
  BACKSLASH_TOK,
  DOT_TOK,
  POUND_TOK,
  QUESTION_MARK_TOK,
  SLASH_TOK,
  STAR_TOK,
  UNDERSCORE_TOK,

  /**
   * @brief Whitespace Tokens.
   */
  SPACE_TOK,
  TAB_TOK,

  /**
   * @brief String Tokens.
   */
  DOUBLE_QUOTATION_TOK,
  LOWER_ALPHA_TOK,
  SINGLE_QUOTATION_TOK,
  TEXT_TOK,
  UPPER_ALPHA_TOK,
  WORD_TOK,

  /**
   * @brief Numeric Tokens.
   */
  FLT_POINT_TOK,
  DIGIT_TOK,
  INT_TOK,

  /**
   * @brief Assignment Tokens.
   */
  EQUAL_TOK,

  /**
   * @brief Mathematical Operator Tokens.
   */
  PLUS_TOK,
  EXPONENTIAL_TOK,
  HYPHEN_TOK,
  MODULUS_TOK,
  REMAINDER_TOK,

  /**
   * @brief Bitwise Operator Tokens.
   */
  BITWISE_AND_TOK,
  BITWISE_OR_TOK,
  BITWISE_SHFT_LEFT_TOK,
  BITWISE_SHFT_RIGHT_TOK,
  BITWISE_TERNARY_TOK,
  BITWISE_XOR_TOK,

  /**
   * @brief Conditional Tokens.
   */
  COND_AND_TOK,
  COND_OR_TOK,
  COND_LTE_TOK,
  COND_GTE_TOK,
  COND_EQUALS_TOK,

  /**
   * @brief Containment Tokens.
   */
  LEFT_CARET_TOK,
  OPEN_CURLY_BRACKET_TOK,
  OPEN_PARENTHESIS_TOK,
  OPEN_SQUARE_BRACKET_TOK,
  CLOSE_CURLY_BRACKET_TOK,
  CLOSE_PARENTHESIS_TOK,
  CLOSE_SQUARE_BRACKET_TOK,
  RIGHT_CARET_TOK,

  /**
   * @brief Architecture Tokens.
   */
  LAMBDA_TOK,

  /**
   * @brief Iterator Tokens.
   */
  DECREMENT_TOK,
  INCREMENT_TOK,

  /**
   * @brief Separator Tokens.
   */
  COLON_TOK,
  COMMA_TOK,

  /**
   * @brief Special Tokens.
   */
  ANNOTATION_TOK,

  /**
   * @brief Reserved Word Tokens.
   */
  ABSTRACT_TOK,
  BOOLEAN_TOK,
  BREAK_TOK,
  CASE_TOK,
  CLASS_TOK,
  CONST_TOK,
  DEFAULT_TOK,
  DOUBLE_TOK,
  EXPORT_TOK,
  FALSE_TOK,
  FLOAT_TOK,
  FOR_TOK,
  IF_TOK,
  IMMUTABLE_TOK,
  IMPORT_TOK,
  INTEGER_TOK,
  IS_TOK,
  MATRIX_TOK,
  NIL_TOK,
  OBJECT_TOK,
  PACKAGE_TOK,
  PRINT_TOK,
  PRIVATE_TOK,
  PROTECTED_TOK,
  PUBLIC_TOK,
  RETURN_TOK,
  SCALAR_TOK,
  SET_TOK,
  STATIC_TOK,
  STRING_TOK,
  SWITCH_TOK,
  TRUE_TOK,
  UNLESS_TOK,
  VECTOR_TOK,
  VOID_TOK,
  WHILE_TOK,
  YIELD_TOK,
};

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
  const char *str;
  syntax_token_t tok;
};

/**
 * @brief
 */
typedef struct syntax_token_insert syntax_token_insert_t;

/**
 * @brief
 */
static const syntax_token_insert_t insertions[] = {
  {
    .str="\0",
    .tok=(syntax_token_t){
      .type=BAD_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="\t",
    .tok=(syntax_token_t){
      .type=TAB_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="\n",
    .tok=(syntax_token_t){
      .type=EOL_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str=" ",
    .tok=(syntax_token_t){
      .type=SPACE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="\"",
    .tok=(syntax_token_t){
      .type=DOUBLE_QUOTATION_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="#",
    .tok=(syntax_token_t){
      .type=POUND_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="\%",
    .tok=(syntax_token_t){
      .type=MODULUS_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="&",
    .tok=(syntax_token_t){
      .type=BITWISE_AND_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="'",
    .tok=(syntax_token_t){
      .type=SINGLE_QUOTATION_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="(",
    .tok=(syntax_token_t){
      .type=OPEN_PARENTHESIS_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str=")",
    .tok=(syntax_token_t){
      .type=CLOSE_PARENTHESIS_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="*",
    .tok=(syntax_token_t){
      .type=STAR_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="+",
    .tok=(syntax_token_t){
      .type=PLUS_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str=",",
    .tok=(syntax_token_t){
      .type=COMMA_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="-",
    .tok=(syntax_token_t){
      .type=HYPHEN_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str=".",
    .tok=(syntax_token_t){
      .type=DOT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="/",
    .tok=(syntax_token_t){
      .type=SLASH_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str=":",
    .tok=(syntax_token_t){
      .type=SLASH_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str=";",
    .tok=(syntax_token_t){
      .type=EOE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="<",
    .tok=(syntax_token_t){
      .type=LEFT_CARET_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="=",
    .tok=(syntax_token_t){
      .type=EQUAL_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str=">",
    .tok=(syntax_token_t){
      .type=RIGHT_CARET_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="?",
    .tok=(syntax_token_t){
      .type=QUESTION_MARK_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="@",
    .tok=(syntax_token_t){
      .type=ANNOTATION_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="[",
    .tok=(syntax_token_t){
      .type=OPEN_SQUARE_BRACKET_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="\\",
    .tok=(syntax_token_t){
      .type=BACKSLASH_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="]",
    .tok=(syntax_token_t){
      .type=CLOSE_SQUARE_BRACKET_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="^",
    .tok=(syntax_token_t){
      .type=BITWISE_XOR_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="_",
    .tok=(syntax_token_t){
      .type=UNDERSCORE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="`",
    .tok=(syntax_token_t){
      .type=APOSTROPHE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="{",
    .tok=(syntax_token_t){
      .type=OPEN_CURLY_BRACKET_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="|",
    .tok=(syntax_token_t){
      .type=BITWISE_OR_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="}",
    .tok=(syntax_token_t){
      .type=CLOSE_CURLY_BRACKET_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="~",
    .tok=(syntax_token_t){
      .type=BITWISE_TERNARY_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="&&",
    .tok=(syntax_token_t){
      .type=COND_AND_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="||",
    .tok=(syntax_token_t){
      .type=COND_OR_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="abstract",
    .tok=(syntax_token_t){
      .type=ABSTRACT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="boolean",
    .tok=(syntax_token_t){
      .type=BOOLEAN_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="break",
    .tok=(syntax_token_t){
      .type=BREAK_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="case",
    .tok=(syntax_token_t){
      .type=CASE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="class",
    .tok=(syntax_token_t){
      .type=CLASS_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="const",
    .tok=(syntax_token_t){
      .type=CONST_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="default",
    .tok=(syntax_token_t){
      .type=DEFAULT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="double",
    .tok=(syntax_token_t){
      .type=DOUBLE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="export",
    .tok=(syntax_token_t){
      .type=EXPORT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="false",
    .tok=(syntax_token_t){
      .type=FALSE_TOK,
      .data=&(int){0},
      .size=sizeof(int),
    },
  }, {
    .str="float",
    .tok=(syntax_token_t){
      .type=FLOAT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="for",
    .tok=(syntax_token_t){
      .type=FOR_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="if",
    .tok=(syntax_token_t){
      .type=IF_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="immutable",
    .tok=(syntax_token_t){
      .type=IMMUTABLE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="import",
    .tok=(syntax_token_t){
      .type=IMPORT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="int",
    .tok=(syntax_token_t){
      .type=INTEGER_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="is",
    .tok=(syntax_token_t){
      .type=IS_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="matrix",
    .tok=(syntax_token_t){
      .type=MATRIX_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="nil",
    .tok=(syntax_token_t){
      .type=NIL_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="object",
    .tok=(syntax_token_t){
      .type=OBJECT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="package",
    .tok=(syntax_token_t){
      .type=PACKAGE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="print",
    .tok=(syntax_token_t){
      .type=PRINT_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="private",
    .tok=(syntax_token_t){
      .type=PRIVATE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="protected",
    .tok=(syntax_token_t){
      .type=PROTECTED_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="public",
    .tok=(syntax_token_t){
      .type=PUBLIC_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="return",
    .tok=(syntax_token_t){
      .type=RETURN_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="scalar",
    .tok=(syntax_token_t){
      .type=SCALAR_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="set",
    .tok=(syntax_token_t){
      .type=SET_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="static",
    .tok=(syntax_token_t){
      .type=STATIC_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="string",
    .tok=(syntax_token_t){
      .type=STRING_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="switch",
    .tok=(syntax_token_t){
      .type=SWITCH_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="true",
    .tok=(syntax_token_t){
      .type=TRUE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="unless",
    .tok=(syntax_token_t){
      .type=UNLESS_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="vector",
    .tok=(syntax_token_t){
      .type=VECTOR_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="void",
    .tok=(syntax_token_t){
      .type=VOID_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="while",
    .tok=(syntax_token_t){
      .type=WHILE_TOK,
      .data=NULL,
      .size=0,
    },
  }, {
    .str="yield",
    .tok=(syntax_token_t){
      .type=YIELD_TOK,
      .data=NULL,
      .size=0,
    },
  },
};

/**
 * @brief
 */
static const size_t count = sizeof(insertions) / sizeof(insertions[0]);

static void lex(syntax_token_trie_t *trie, const char *data)
{
  const char **ptr = &data;
  syntax_token_t rettok, *retptr = &rettok;

  do
  {
    const char *start = *ptr;

    /**
     * NOTE: If the literal search does not work attempt a pattern search.
     */
    if (syntax_token_trie_search(trie, retptr, ptr) == false)
    {
      *ptr = start;

      if (ae_match(*data, ae_is_space))
      {
        handle_whitespace(queue, &token, &data);
      }

      else if (ae_match(*data, ae_is_digit))
      {
        handle_number(queue, &token, &data);
      }

      else if (ae_match(*data, ae_is_alnum))
      {
        handle_word(queue, &token, &data);
      }

      else if (ae_match(*data, ae_is_symbl))
      {
        handle_symbol(queue, &token, &data);
      }

      else
      {
        throw(X_ERROR_UNSUPPORTED_CHARACTER);
      }

      // if (isalpha(**ptr))
      // {
      //   rettok.type = WORD_TOK;

      //   rettok.data = __malloc(128 * sizeof(char));
      //   memset(rettok.data, 0, 128 * sizeof(char));

      //   char *p = rettok.data;
      //   for (; **ptr && isalpha(**ptr); (*ptr)++, rettok.size++)
      //   {
      //     *(p++) = **ptr;
      //   }
      // }
    }

    switch (rettok.type)
    {
      case BAD_TOK:
        puts("BAD TOKEN");
        break;

      case SPACE_TOK:
        puts("SPACE TOKEN");
        break;

      case EQUAL_TOK:
        puts("EQUAL TOKEN");
        break;

      case BITWISE_OR_TOK:
        puts("BITWISE OR TOKEN");
        break;

      case COND_OR_TOK:
        puts("CONDITIONAL OR TOKEN");
        break;

      case INTEGER_TOK:
        puts("INTEGER TOKEN");
        break;

      case EOE_TOK:
        puts("EOE TOKEN");
        break;

      case WORD_TOK:
        puts("WORD TOKEN");
        break;

      case BOOLEAN_TOK:
        puts("BOOLEAN TOKEN");
        break;
    }
  }
  while (**ptr);
}

int init(void)
{
  /* NOTE: Allocates the global token dictionary to the heap. */
  global_token_diction = syntax_token_trie_new();

  /* NOTE: Adds all tokens to the global token dictionary. */
  for (uint64_t i = 0; i < count; i++)
  {
    syntax_token_trie_insert(global_token_diction,   insertions[i].str,
                                                    &insertions[i].tok);
  }
}

int main(void)
{
  init();

  const char data[] = "boolean n = x || y;";

  lex(global_token_diction, data);

  /* NOTE: Deallocate the global token dictionary from the heap. */
  syntax_token_trie_destroy(global_token_diction);

  return EXIT_SUCCESS;
}
