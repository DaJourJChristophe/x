#ifndef X_SYNTAX_FACTS_H
#define X_SYNTAX_FACTS_H

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
  NUM_TOK,

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
 * @brief Define a collection of return types.
 */
enum
{
  BOOLEAN_RETURN_TYPE,
  DOUBLE_RETURN_TYPE,
  FLOAT_RETURN_TYPE,
  INTEGER_RETURN_TYPE,
  MATRIX_RETURN_TYPE,
  NIL_RETURN_TYPE,
  NUMBER_RETURN_TYPE,
  OBJECT_RETURN_TYPE,
  SCALAR_RETURN_TYPE,
  STRING_RETURN_TYPE,
  VECTOR_RETURN_TYPE,
  VOID_RETURN_TYPE,
};

// /**
//  * @brief Define a collection of character tokens.
//  */
// enum
// {
//   /**
//    * @brief General Purpose Tokens.
//    */
//   DOT,
//   EOE,
//   LEXER_EOF,
//   EOL,
//   SPACE,
//   STAR,
//   SYMBOL,
//   TAB,
//   TEXT,
//   UNDEFINED,
//   WORD,

//   /**
//    * @brief Numeric Tokens.
//    */
//   DECIMAL,
//   NUMBER,
//   INTEGER_VALUE,

//   /**
//    * @brief Mathematical Operator Tokens.
//    */
//   ADDITION,
//   DIVISION,
//   EQUAL,
//   EXPONENTIAL,
//   MODULUS,
//   REMAINDER,
//   SUBTRACTION,

//   /**
//    * @brief Bitwise Operator Tokens.
//    */
//   BITWISE_AND,
//   BITWISE_OR,
//   BITWISE_TERNARY,
//   BITWISE_XOR,
//   BITWISE_SHIFT_LEFT,
//   BITWISE_SHIFT_RIGHT,

//   /**
//    * @brief Conditional Tokens.
//    */
//   CONDITIONAL_AND,
//   CONDITIONAL_OR,

//   /**
//    * @brief Containment Tokens.
//    */
//   LEFT_CARET,
//   OPEN_CURLY_BRACKET,
//   OPEN_PARENTHESIS,
//   OPEN_SQUARE_BRACKET,
//   CLOSE_CURLY_BRACKET,
//   CLOSE_PARENTHESIS,
//   CLOSE_SQUARE_BRACKET,
//   RIGHT_CARET,

//   /**
//    * @brief Architecture Tokens.
//    */
//   LAMBDA,

//   /**
//    * @brief Comparator Tokens.
//    */
//   EQUALS,

//   /**
//    * @brief Iterator Tokens.
//    */
//   DECREMENT,
//   INCREMENT,

//   /**
//    * @brief Separator Tokens.
//    */
//   COLON,
//   COMMA,

//   /**
//    * @brief Special Tokens.
//    */
//   ANNOTATION,

//   /**
//    * @brief Reserved Word Tokens.
//    */
//   ABSTRACT,
//   BOOLEAN,
//   BREAK,
//   CASE,
//   CLASS,
//   CONST,
//   DEFAULT,
//   DOUBLE,
//   EXPORT,
//   FALSE,
//   FLOAT,
//   FOR,
//   IF,
//   IMMUTABLE,
//   IMPORT,
//   INTEGER,
//   IS,
//   MATRIX,
//   NIL,
//   OBJECT,
//   PACKAGE,
//   PRINT,
//   PRIVATE,
//   PROTECTED,
//   PUBLIC,
//   RETURN,
//   SCALAR,
//   SET,
//   STATIC,
//   STRING,
//   SWITCH,
//   TRUE,
//   UNLESS,
//   VECTOR,
//   VOID,
//   WHILE,
//   YIELD,
// };

// /**
//  * @brief Reserved Words.
//  */
// enum
// {
//   ABSTRACT_RESERVED_WORD = 7572149969927417,
//   BOOLEAN_RESERVED_WORD = 229460885180549,
//   BREAK_RESERVED_WORD = 210707980106,
//   CASE_RESERVED_WORD = 6385108193,
//   CLASS_RESERVED_WORD = 210708946651,
//   CONST_RESERVED_WORD = 210709068620,
//   DEFAULT_RESERVED_WORD = 229463065711754,
//   DOUBLE_RESERVED_WORD = 6953438632736,
//   EXPORT_RESERVED_WORD = 6953488276103,
//   FALSE_RESERVED_WORD = 210712121072,
//   FLOAT_RESERVED_WORD = 210712519067,
//   FOR_RESERVED_WORD = 193491852,
//   IF_RESERVED_WORD = 5863476,
//   IMMUTABLE_RESERVED_WORD = 249892661372121605,
//   IMPORT_RESERVED_WORD = 6953631772544,
//   INTEGER_RESERVED_WORD = 193495088,
//   IS_RESERVED_WORD = 5863489,
//   MATRIX_RESERVED_WORD = 6953774229786,
//   NIL_RESERVED_WORD = 6385525056,
//   OBJECT_RESERVED_WORD = 6953853312764,
//   PACKAGE_RESERVED_WORD = 229478403565457,
//   PRINT_RESERVED_WORD = 210724587794,
//   PRIVATE_RESERVED_WORD = 229479076378400,
//   PROTECTED_RESERVED_WORD = 249902721850767151,
//   PUBLIC_RESERVED_WORD = 6953914700964,
//   RETURN_RESERVED_WORD = 6953974653989,
//   SCALAR_RESERVED_WORD = 6954010724379,
//   SET_RESERVED_WORD = 193505681,
//   STATIC_RESERVED_WORD = 6954030893997,
//   STRING_RESERVED_WORD = 6954031493116,
//   SWITCH_RESERVED_WORD = 6954034739063,
//   TRUE_RESERVED_WORD = 6385737701,
//   UNLESS_RESERVED_WORD = 6954102428575,
//   VECTOR_RESERVED_WORD = 6954130583448,
//   VOID_RESERVED_WORD = 6385805911,
//   WHILE_RESERVED_WORD = 210732529790,
//   YIELD_RESERVED_WORD = 210734933212,
// };

/**
 * @brief Define a collection of return types.
 */
int precedence(int kind);

#endif/*X_SYNTAX_FACTS_H*/
