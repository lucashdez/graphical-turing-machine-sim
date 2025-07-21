@echo off
if not exist "./build" mkdir build
pushd build
clang -g -I"../include" -o main.exe ../src/main.c -l ../lib/glfw-win/glfw3dll -l ../lib/vulkan/vulkan-1
copy ..\lib\glfw-win\glfw3.dll .
popd