#include "memory.h"

function void* 
mm_malloc_reserve(void* ctx, u64 size) {
    return (malloc(size));
}

function void
mm_release(void* ctx, void* ptr, u64 size) {
    free(ptr);
}

function struct MM_BaseMemory*
mm_create_malloc_base_memory(void) {
    local struct MM_BaseMemory memory = {};
    if (memory.reserve == 0) {
        memory.reserve = mm_malloc_reserve;
        memory.commit = mm_change_memory_noop;
        memory.decommit = mm_change_memory_noop;
        memory.release = mm_release;
    }
    return(&memory); 
}


function struct Arena 
mm_make_arena_reserve(struct MM_BaseMemory *base, u64 reserve_size) {
    struct Arena result = {};
    result.base = base;
    result.memory = base->reserve(base->ctx, reserve_size);
    result.cap = reserve_size;
    return (result); 
}

function struct Arena
mm_make_arena(struct MM_BaseMemory *base) {
    struct Arena result =  mm_make_arena_reserve(base, KB(16));
    return (result);
}

function void 
mm_arena_release(struct Arena *arena) {
    struct MM_BaseMemory *base = arena->base;
    base->release(base->ctx, arena->memory, arena->cap);
} 

function void* 
mm_arena_push(struct Arena *arena, u64 size) {
    void* result = NULL;
    if (arena->pos + size <= arena->cap) {
        result = arena->memory + arena->pos;
        arena->pos += size;
        if (arena->pos >= arena->commit_pos) {
            u64 next_commit_p = align2pow(arena->pos, MM_COMMIT_BLOCK_SIZE - 1);
            u64 next_commit_p_clamped = clampTop(next_commit_p, arena->cap);
            struct MM_BaseMemory *base = arena->base; 
            base->commit(base->ctx, arena->memory + arena->commit_pos, size);
        }
    }
    
    return (result);
}