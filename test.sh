#!/bin/bash

set -e

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/peter/Documents/x/libexec/

# ./bin/ae_test
./bin/stack_test
