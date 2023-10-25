#!/bin/sh

set -e

trap 'rm -f .tmp.c .tmp.o .tmp.json.part' EXIT

echo 'int main(void){}' > .tmp.c
clang -Wall -Wextra -Werror -std=c99 -pedantic -Iinclude -Isrc/include -c -xc -MJ .tmp.json.part -o .tmp.o .tmp.c
(echo '[' && cat .tmp.json.part | rev | cut -c 2- | rev && echo ']') > compile_commands.json
