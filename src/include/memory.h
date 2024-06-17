/* date = June 15th 2024 1:02 pm */

#ifndef MEMORY_H
#define MEMORY_H
// mm for Memory Manager



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

function struct MM_BaseMemory* mm_create_malloc_base_memory(void);
function void mm_change_memory_noop(void* ctx, void* ptr, u64 size) {}
function struct Arena mm_make_arena_reserve(struct MM_BaseMemory *base, u64 reserve_size);
function struct Arena mm_make_arena(struct MM_BaseMemory *base);

function void mm_arena_release(struct Arena *arena);
function void* mm_arena_push(struct Arena *arena, u64 size);
function void mm_arena_pop_to(struct Arena *arena, u64 pos);

#endif //MEMORY_H