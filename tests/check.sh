#!/bin/bash

set -e
cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1

./external_dependencies/fetch_all.sh
cc \
   -g -Wall -Wextra -pedantic -std=c99 \
   -I../ \
  -I./external_dependencies/jsmn-jsonpath \
  -I./external_dependencies/toolbox \
  -I./external_dependencies/jsmn \
  -DJSMN_PARENT_LINKS \
  -o test \
  test.c \
  ../s_json.c \
  external_dependencies/jsmn/jsmn.c \
  external_dependencies/jsmn-jsonpath/jsonpath.c \
  external_dependencies/jsmn-jsonpath/jjp_wrap.c
./test
echo "All tests were successfull :):):)"
