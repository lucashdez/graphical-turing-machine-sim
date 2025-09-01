mkdir -p build

gcc -g -I"./include" -o ./build/main ./src/main.c -l vulkan -l glfw -lc -lwayland-client