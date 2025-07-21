#ifndef MEMORY_H
#  define MEMORY_H
// mm for Memory Manager
#include <stdlib.h>

typedef void* MM_ReserveFunc(void* ctx, u64 size);
typedef void MM_ChangeMemoryFunc(void* ctx, void* ptr, u64 size);

typedef struct MM_BaseMemory {
    MM_ReserveFunc *reserve;
    MM_ChangeMemoryFunc *commit;
    MM_ChangeMemoryFunc *decommit;
    MM_ChangeMemoryFunc *release;
    void *ctx;
} MM_BaseMemory;

typedef struct Arena {
    struct MM_BaseMemory* base;
    u8* memory; //pointer to the initial
    u64 cap;
    u64 pos;
    u64 commit_pos;
} Arena;

// UTILITIES
static void* mm_memset(void* dest, register int val, register u64 len);
static void* mm_memcpy(void* dest, void* src, u64 len);
static void* mm_memcpymv(void* dest, void* src, u64 len);

static MM_BaseMemory* mm_create_malloc_base_memory(void);
static void mm_change_memory_noop(void* ctx, void* ptr, u64 size) {}
static Arena mm_make_arena_reserve(MM_BaseMemory *base, u64 reserve_size);
static Arena mm_make_arena(MM_BaseMemory *base);
static Arena mm_scratch_arena();


static void mm_arena_release(Arena *arena);
static void* mm_arena_push(Arena *arena, u64 size);
static void* mm_arena_push_zeros(Arena *arena, u64 size);
static void mm_arena_pop_to(Arena *arena, u64 pos);


// GLOBALS
static MM_BaseMemory* GLOBAL_BASE_ALLOCATOR = {0};

#endif