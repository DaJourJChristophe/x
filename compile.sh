#!/bin/bash

set -e

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/peter/Documents/x/libexec/

# Library Source Build

# gcc -c -Iinclude -std=c99 -pedantic -Wall -Wextra -Werror \
#   -ggdb3 -DNDEBUG -O0 -fPIC -o ./src/ae.o ./src/ae.c

gcc -c -Iinclude -std=c99 -pedantic -Wall -Wextra -Werror \
  -ggdb3 -DNDEBUG -O0 -fPIC -o ./src/cache.o ./src/cache.c

gcc -c -Iinclude -std=c99 -pedantic -Wall -Wextra -Werror \
  -ggdb3 -DNDEBUG -O0 -fPIC -o ./src/error.o ./src/error.c

gcc -c -Iinclude -std=c99 -pedantic -Wall -Wextra -Werror \
  -ggdb3 -DNDEBUG -O0 -fPIC -o ./src/io.o ./src/io.c

# INFO: Cannot ISO C do to reserved word enums too large.
gcc -c -Iinclude -std=c99 -Wall -Wextra -Werror \
  -ggdb3 -DNDEBUG -O0 -fPIC -o ./src/lexer.o ./src/lexer.c

gcc -c -Iinclude -std=c99 -Wall -Wextra -Werror \
  -ggdb3 -DNDEBUG -O0 -fPIC -o ./src/parse.o ./src/parse.c

gcc -c -Iinclude -std=c99 -Wall -Wextra -Werror \
  -ggdb3 -DNDEBUG -O0 -fPIC -o ./src/queue.o ./src/queue.c

gcc -c -Iinclude -std=c99 -pedantic -Wall -Wextra -Werror \
  -ggdb3 -DNDEBUG -O0 -fPIC -o ./src/utils.o ./src/utils.c

# Program Source Build

gcc -c -Iinclude -std=c99 -pedantic -Wall -Wextra -Werror \
  -ggdb3 -DNDEBUG -O0 -o ./src/main.o ./src/main.c


# Test Source Build

gcc -c -Iinclude -Iinclude/cmocka -std=c99 -pedantic \
  -Wall -Wextra -Werror -ggdb -DNDEBUG -O0 \
  -o ./test/ae_test.o ./test/ae_test.c


# Link Library

gcc -Llibexec -shared -o ./libexec/libx.so \
  ./src/cache.o \
  ./src/error.o \
  ./src/io.o \
  ./src/lexer.o \
  ./src/parse.o \
  ./src/queue.o \
  ./src/utils.o


# Link Program

gcc -Llibexec -o ./bin/main ./src/main.o -lx


# Link Test

gcc -Llibexec -o ./bin/ae_test ./test/ae_test.o -lx -lcmocka


# Clean

rm -rf ./src/*.o ./test/*o
