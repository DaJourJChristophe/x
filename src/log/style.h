#ifndef X_LOG_STYLE_H
#define X_LOG_STYLE_H

#include <stddef.h>

/**
 * @brief Define a collection of general purpose text styling attributes.
 */
enum
{
  RESET_TEXT_STYLE = 0,
};

/**
 * @brief Define a collection of text styling attributes.
 */
enum
{
  /**
   * @brief Define a collection of test styles.
   */
  NORMAL_TEXT_STYLE = 0,
  BOLD_TEXT_STYLE = 1,
  OPACITY_TEXT_STYLE = 2,
  ITALIC_TEXT_STYLE = 3,
  UNDERLINE_TEXT_STYLE = 4,

  /**
   * @brief Define a collection of test colors.
   */
  RESET_TEXT_COLOR = 0,
  RED_TEXT_COLOR = 31,
  GREEN_TEXT_COLOR = 32,
  YELLOW_TEXT_COLOR = 33,
  BLUE_TEXT_COLOR = 34,
  PURPLE_TEXT_COLOR = 35,
  TURQUOISE_TEXT_COLOR = 36,
  WHITE_TEXT_COLOR = 37,
  GREY_TEXT_COLOR = 39,

  /**
   * @brief Define a collection of test background colors.
   */
  RESET_TEXT_BACKGROUND_COLOR = 41,
  RED_TEXT_BACKGROUND_COLOR = 41,
  GREEN_TEXT_BACKGROUND_COLOR = 42,
  YELLOW_TEXT_BACKGROUND_COLOR = 43,
  BLUE_TEXT_BACKGROUND_COLOR = 44,
  PURPLE_TEXT_BACKGROUND_COLOR = 45,
  TURQUOISE_TEXT_BACKGROUND_COLOR = 46,
  WHITE_TEXT_BACKGROUND_COLOR = 47,
  GREY_TEXT_BACKGROUND_COLOR = 49,
};

/**
 * @brief Define a text style data structure.
 */
struct text_style
{
  int bg_color;
  int color;
  int style;
};

/**
 * @brief Define a namespace for the text style data structure.
 */
typedef struct text_style text_style_t;

/**
 * @brief Define the text style max. buffer.
 */
#define TEXT_STYLE_MAXBUF   1024

/**
 * @brief
 */
void compile(char *buffer, const size_t size, text_style_t *style);

#endif/*X_LOG_STYLE_H*/
