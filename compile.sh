#!/bin/bash

set -e

# Library Source Build

gcc -c -Iinclude -std=c99 -pedantic -Wall -Wextra -Werror \
  -ggdb -DNDEBUG -O0 -fPIC -o ./src/ae.o ./src/ae.c

gcc -c -Iinclude -std=c99 -pedantic -Wall -Wextra -Werror \
  -ggdb -DNDEBUG -O0 -fPIC -o ./src/io.o ./src/io.c

gcc -c -Iinclude -std=c99 -pedantic -Wall -Wextra -Werror \
  -ggdb -DNDEBUG -O0 -fPIC -o ./src/lexer.o ./src/lexer.c

gcc -c -Iinclude -std=c99 -pedantic -Wall -Wextra -Werror \
  -ggdb -DNDEBUG -O0 -fPIC -o ./src/re.o ./src/re.c

# Program Source Build

gcc -c -Iinclude -std=c99 -pedantic -Wall -Wextra -Werror \
  -ggdb -DNDEBUG -O0 -o ./src/main.o ./src/main.c


# Test Source Build

gcc -c -Iinclude -Iinclude/cmocka -std=c99 -pedantic \
  -Wall -Wextra -Werror -ggdb -DNDEBUG -O0 \
  -o ./test/ae_test.o ./test/ae_test.c


# Link Library

gcc -Llibexec -shared -o ./libexec/libx.so \
  ./src/ae.o \
  ./src/io.o \
  ./src/lexer.o \
  ./src/re.o


# Link Program

gcc -Llibexec -o ./bin/main ./src/main.o -lx


# Link Test

gcc -Llibexec -o ./bin/ae_test ./test/ae_test.o -lx -lcmocka


# Clean

rm -rf ./src/*.o ./test/*o
