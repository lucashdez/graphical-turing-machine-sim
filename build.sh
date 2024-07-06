#!/bin/bash

code="$PWD"
opts=-g
cd build > /dev/null
clang -Werror $opts -I"$code/src/include" -I"$code/src/impls/" $code/src/main.c -lX11 -o gtms
cd $code > /dev/null
