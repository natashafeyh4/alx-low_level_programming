#!/bin/bash
gcc -Wall -pedantic -Werror -Wextra -c *.c
gcc -shared -o liball.so *.o
rm *.o
