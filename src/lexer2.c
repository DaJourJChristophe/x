
#include <stdint.h>
#include <stdio.h>
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

/**
 * @brief Define the maximum depth of a character combination.
 */
#define MAX_COMB_DEPTH   ((int)3)

/**
 * @brief Define the maximum possible character value range.
 */
#define MAX_CHAR_RNG     ((int)128)

/**
 * @brief
 */
static const int chrtbl[MAX_COMB_DEPTH][MAX_CHAR_RNG] = {
  {
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    TAB_TOK,
    EOL_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    SPACE_TOK,
    BAD_TOK,
    DOUBLE_QUOTATION_TOK,
    POUND_TOK,
    BAD_TOK,
    MODULUS_TOK,
    BITWISE_AND_TOK,
    SINGLE_QUOTATION_TOK,
    OPEN_PARENTHESIS_TOK,
    CLOSE_PARENTHESIS_TOK,
    STAR_TOK,
    PLUS_TOK,
    COMMA_TOK,
    HYPHEN_TOK,
    DOT_TOK,
    SLASH_TOK,
    DIGIT_TOK,
    DIGIT_TOK,
    DIGIT_TOK,
    DIGIT_TOK,
    DIGIT_TOK,
    DIGIT_TOK,
    DIGIT_TOK,
    DIGIT_TOK,
    DIGIT_TOK,
    DIGIT_TOK,
    COLON_TOK,
    EOE_TOK,
    LEFT_CARET_TOK,
    EQUAL_TOK,
    RIGHT_CARET_TOK,
    QUESTION_MARK_TOK,
    ANNOTATION_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    UPPER_ALPHA_TOK,
    OPEN_SQUARE_BRACKET_TOK,
    BACKSLASH_TOK,
    CLOSE_SQUARE_BRACKET_TOK,
    BITWISE_XOR_TOK,
    UNDERSCORE_TOK,
    APOSTROPHE_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    LOWER_ALPHA_TOK,
    OPEN_CURLY_BRACKET_TOK,
    BITWISE_OR_TOK,
    CLOSE_CURLY_BRACKET_TOK,
    BITWISE_TERNARY_TOK,
    BAD_TOK,
  }, {
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    COND_AND_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    LEFT_CARET_TOK,
    COND_EQUALS_TOK,
    RIGHT_CARET_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    COND_OR_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
  },{
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
    BAD_TOK,
  },
};

static int gettok(const char *data)
{
  const int n = MAX_COMB_DEPTH;
  int selection = BAD_TOK;
  int previous_selection = BAD_TOK;
  int i = 0;

  do
  {
    selection = chrtbl[i++][(int)(*(data++))];

    switch (selection)
    {
      case EQUAL_TOK:
        /**
         * NOTE: Implement backtracking here .. :P
         */
        switch (previous_selection)
        {
          case LEFT_CARET_TOK:
            selection = COND_LTE_TOK;
            break;

          case EQUAL_TOK:
            selection = COND_EQUALS_TOK;
            break;

          case RIGHT_CARET_TOK:
            selection = COND_GTE_TOK;
            break;
        }
        break;
    }

    previous_selection = selection;
  }
  while (*data && i < n && selection);

  return selection;
}

int main(void)
{
  int selection = gettok("==");
  if (selection == 0)
  {
    fprintf(stderr, "\033[0;31m[fail] FatalError: %s\n\nAborted\n\033[0m", "Encountered an unsupported character.");
  }
  return EXIT_SUCCESS;
}
