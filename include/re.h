#ifndef X_REGULAR_EXPRESSION_H
#define X_REGULAR_EXPRESSION_H

/*
 * Define to 0 if you DON'T want '.' to match '\r' + '\n'
 */
#ifndef RE_DOT_MATCHES_NEWLINE
#define RE_DOT_MATCHES_NEWLINE   1
#endif/*RE_DOT_MATCHES_NEWLINE*/

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief Pointer to get abstract datatype.
 */
typedef struct regex_t *re_t;

/**
 * @brief Compile regex string pattern to a regex_t-array.
 *
 */
re_t re_compile(const char *pattern);

/**
 * @brief Find matches of the compiled pattern inside text.
 */
int re_match_compiled(re_t pattern, const char *text, int *length);

/**
 * @brief Find matches of the txt pattern inside text
 *        (will compile automatically first).
 *
 */
int re_match(const char *pattern, const char *text, int *length);

#ifdef __cplusplus
}
#endif

#endif/*X_REGULAR_EXPRESSION_H*/
