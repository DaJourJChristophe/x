#### ASCII Evaluation

ASCII Evaluation is a process that categorizes a single character. Unlike ASCII Expressions or Regular Expressions, ASCII Evaluation simple performs a comparison or a single character based upon a predetermined contextual concept. For example, given the question, "<em>is a character alphanumeric</em>", perform a combination of comparison to determine if the character is indeed alphanumeric.

##### Supported ASCII Evaluations

The following methods are bitwise evaluations that perform a bitwise comparison between the variable `c` and a const evaluation definition.

<br />

Check if a character is alphabetic.
```c
bool is_alpha(const char c);
```

Check if a character is alphanumeric.
```c
bool is_alphanum(const char c);
```

Check if a character is ASCII.
```c
bool is_ascii(const char c);
```

Check if a character is a digit.
```c
bool is_digit(const char c);
```

Check if a character is a lower case.
```c
bool is_lower(const char c);
```

Check if a character is a upper case.
```c
bool is_upper(const char c);
```

Check if a character is whitespace.
```c
bool is_white(const char c);
```

Check if a character matches the expression definition.
```c
bool ae_match(const char c, const int expression);
```
