// HEADERS
#include "base.h"
#include "win32.h"
#include <stdlib.h>
#include <stdio.h>

//    PLATFORM
#include "win32.h"

// DEFINITIONS
#include "memory.c"
#include "win32.c"

//    PLATFORM

enum Week_days {
    WD_Monday
};
#ifdef __linux__
int main() {
    struct MM_BaseMemory* mem = mm_create_malloc_base_memory();
}
#endif

function i32 _start_function() {
    printf("Hello, both\n");
    return(0);
}

