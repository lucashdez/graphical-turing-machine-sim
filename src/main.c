<<<<<<< Updated upstream
// HEADERS
#include "base.h"
#include "win32.h"
#include <stdlib.h>
=======
// NOTE(lucashdez): Headers
#include "base.h"
>>>>>>> Stashed changes
#include <stdio.h>
// NOTE(lucashdez): sources

// NOTE(lucashdez): Platform specific
#include "win32.h"
#include "win32.c"

// DEFINITIONS
#include "memory.c"

<<<<<<< Updated upstream
enum Week_days {
    WD_Monday
};

int main() {
    struct MM_BaseMemory* mem = mm_create_malloc_base_memory();
=======
function i32 _start_function() {
    printf("Hello, both\n");
>>>>>>> Stashed changes
    return(0);
}