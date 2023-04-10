#ifndef X_ERROR_H
#define X_ERROR_H

#define X_ERROR_UNSUPPORTED_CHARACTER "The compiler encountered an unsupported character"

#define X_ERROR_SYNTAX_QUEUE_WRITE    "Could not write to the syntax queue"

void throw(char const *errmsg);

#endif/*X_ERROR_H*/
