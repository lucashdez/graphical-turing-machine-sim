@echo off
set opts=-g
set code=%cd%
pushd build
clang %opts% -I"%code%\src\include" %code%\src\main.c -o gtms.exe
popd
