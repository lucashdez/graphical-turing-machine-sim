/* date = June 15th 2024 1:02 pm */

#ifndef MEMORY_H
#define MEMORY_H
// mm for Memory Manager
#include <stdlib.h>

typedef void* MM_ReserveFunc(void* ctx, u64 size);
typedef void MM_ChangeMemoryFunc(void* ctx, void* ptr, u64 size);

struct MM_BaseMemory {
    MM_ReserveFunc *reserve;
    MM_ChangeMemoryFunc *commit;
    MM_ChangeMemoryFunc *decommit;
    MM_ChangeMemoryFunc *release;
    void *ctx;
};

struct Arena {
    struct MM_BaseMemory* base;
    u8* memory; //pointer to the initial
    u64 cap;
    u64 pos;
    u64 commit_pos;
};

// UTILITIES
static void* mm_memset(void* dest, register int val, register u64 len);
static void* mm_memcpy(void* dest, void* src, u64 len);
static void* mm_memcpymv(void* dest, void* src, u64 len);

static struct MM_BaseMemory* mm_create_malloc_base_memory(void);
static void mm_change_memory_noop(void* ctx, void* ptr, u64 size) {}
static struct Arena mm_make_arena_reserve(struct MM_BaseMemory *base, u64 reserve_size);
static struct Arena mm_make_arena(struct MM_BaseMemory *base);
static struct Arena mm_scratch_arena();


static void mm_arena_release(struct Arena *arena);
static void* mm_arena_push(struct Arena *arena, u64 size);
static void* mm_arena_push_zeros(struct Arena *arena, u64 size);
static void mm_arena_pop_to(struct Arena *arena, u64 pos);


// GLOBALS
static struct MM_BaseMemory* GLOBAL_BASE_ALLOCATOR = {0};

#endif //MEMORY_H
