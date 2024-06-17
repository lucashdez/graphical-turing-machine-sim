@echo off
set opts=-g
set code=%cd%
pushd build
clang %opts% -I"%code%\src\include" -I"%code%\src\impls" %code%\src\main.c -o gtms.exe
popd
