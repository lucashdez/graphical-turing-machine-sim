#!/bin/bash

code="$PWD"
opts=-g
cd build > /dev/null
g++ $opts $code/./src/main.c -o gtms.exe
cd $code > /dev/null
