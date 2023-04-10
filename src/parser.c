#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* START TEMPORARY COMMON.H */

#define always_inline __attribute__ ((always_inline))

static inline void * always_inline __malloc(size_t const size)
{
  void *p = NULL;
  p = malloc(size);
  if (p == NULL)
  {
    fprintf(stderr, "%s\n", "memory error");
    exit(EXIT_FAILURE);
  }
  return p;
}

static inline void * always_inline __calloc(size_t const n, size_t const size)
{
  void *p = NULL;
  p = calloc(n, size);
  if (p == NULL)
  {
    fprintf(stderr, "%s\n", "memory error");
    exit(EXIT_FAILURE);
  }
  return p;
}

static inline void always_inline __free(void *p)
{
  if (p != NULL)
  {
    free(p);
    p = NULL;
  }
}

/* END   TEMPORARY COMMON.H */

/**
 * @brief A collection of syntax expression datatypes.
 */
enum
{
  BinaryExpression,
  NumberExpression,
};

struct token
{
   int  type;      /* Token datatype declaration. */
  void *data;      /* Data associated with the corresponding datatype. */

  /**
   * NOTE: Potentially will not apply once data is allocated and casted.
   */
  const char *i;   /* Inclusive starting character of token. */
  const char *j;   /* Exclusive ending   character of token. */
};

/**
 * @brief Define a namespace for the token structure.
 */
typedef struct token token_t;

/**
 * @brief Define a namespace for the syntax token structure.
 */
typedef token_t syntax_token_t;

/**
 * @brief Allocate a new syntax token and set both the type and the data;
 */
syntax_token_t *syntax_token_new(int type, void *data, size_t const n)
{
  syntax_token_t *token = NULL;
  token = __calloc(1, sizeof(syntax_token_t));

  token->type = type;
  token->data = __malloc(n);

  memcpy(token->data, data, n);

  return token;
}

/**
 * @brief Destroy the token and the token data.
 */
void syntax_token_destroy(syntax_token_t *token)
{
  __free(token->data);
  __free(token);
}

struct syntax_node
{
  syntax_token_t *value;       /* A data structure containing the type and data for the syntax token. */

  struct syntax_node *left;    /* A left  child pointer to a syntax node. */
  struct syntax_node *right;   /* A right child pointer to a syntax node. */
};

/**
 * @brief Define a namespace for the syntax node structure.
 */
typedef struct syntax_node syntax_node_t;

/**
 * @brief Define a namespace for the syntax expression structure.
 */
typedef syntax_node_t syntax_expression_t;

/**
 * @brief Allocate a new syntax expression and set the value, left child, and right child.
 */
syntax_expression_t *expression_new(syntax_token_t *value, syntax_expression_t *left, syntax_expression_t *right)
{
  syntax_expression_t *expression = NULL;
  expression = __calloc(1, sizeof(syntax_expression_t));

  expression->value = value;
  expression->left = left;
  expression->right = right;

  return expression;
}

/**
 * @brief Deallocate the expression value and the expression data structure.
 */
void expression_destroy(syntax_expression_t *expression)
{
  __free(expression->value);
  __free(expression);
}

/**
 * @brief Define a namespace for the number expression structure.
 */
typedef syntax_expression_t number_expression_t;

/**
 * @brief Allocate a new syntax expression, set the value, and set both the left and right child pointers to null.
 */
number_expression_t *number_expression_new(syntax_token_t *value)
{
  return expression_new(value, NULL, NULL);
}

/**
 * @brief Define a namespace for the binary expression structure.
 */
typedef syntax_expression_t binary_expression_t;

/**
 * @brief Allocate a new syntax expression, set the operator token, and set both the left and the right expressions.
 */
binary_expression_t *binary_expression_new(syntax_token_t *operator, syntax_token_t *left, syntax_token_t *right)
{
  return expression_new(operator, number_expression_new(left), number_expression_new(right));
}

int main(void)
{
  int a = 5;
  int b = 3;

  syntax_token_t *token_o = syntax_token_new(3, NULL, 0);
  syntax_token_t *token_a = syntax_token_new(5, &a, sizeof(int));
  syntax_token_t *token_b = syntax_token_new(5, &b, sizeof(int));

  binary_expression_t *expression = binary_expression_new(token_o, token_a, token_b);

  printf("%d\n", expression->value->type);
  printf("%d\n", *(int *)expression->value->data);

  expression_destroy(expression);

  return EXIT_SUCCESS;
}
