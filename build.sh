mkdir -p build

clang -g -I"./include" -o ./build/main ./src/main.c -l vulkan -l glfw