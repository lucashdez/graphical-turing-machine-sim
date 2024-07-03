// HEADERS
#include "base/base_include.h"
#include <stdlib.h>
#include <stdio.h>

//    PLATFORM
#include "win32.h"

// DEFINITIONS
#include "win32.c"

//    PLATFORM

enum Week_days {
    WD_Monday
};
#ifdef __linux__
int main(int argc, char **argv) {
    struct MM_BaseMemory* mem = mm_create_malloc_base_memory();
}
#endif

function i32 _start_function() {
    printf("Hello, both\n");
    return(0);
}

