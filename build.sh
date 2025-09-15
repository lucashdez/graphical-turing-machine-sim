mkdir -p build

gcc -g -I"./include" -o ./build/main ./src/main.c -l vulkan -lc -lwayland-client
