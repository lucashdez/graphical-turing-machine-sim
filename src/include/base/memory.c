#include "memory.h"

static void* 
mm_memset(void* dest, register int val, register u64 len) {
    register u8* ptr = (u8*)dest;
    while(len-- > 0) {
        *ptr++ = val;
    }
    return dest;
}

static void* 
mm_memcpy(void* dest, void* src, u64 len) {
    char *d = dest;
    const char *s = src;
    while(len-- && s+1 != 0) {
        *d++ = *s++;
    }
    return dest;
}

static void* 
mm_memcpymv(void* dest, void* src, u64 len) {
    char *d = dest;
    const char *s = src;
    while(len-- && *(s+1) != 0) {
        *d++ = *s++;
        dest = (void*)d;
    }
    return dest;
}

// TODO Own Memory management with virtualalloc

static void* 
mm_malloc_reserve(void* ctx, u64 size) {
    return (malloc(size));
}

static void
mm_release(void* ctx, void* ptr, u64 size) {
    free(ptr);
}

static struct MM_BaseMemory*
mm_create_malloc_base_memory(void) {
    static struct MM_BaseMemory memory = {};
    if (memory.reserve == 0) {
        memory.reserve = mm_malloc_reserve;
        memory.commit = mm_change_memory_noop;
        memory.decommit = mm_change_memory_noop;
        memory.release = mm_release;
    }
    return(&memory); 
}


static struct Arena 
mm_make_arena_reserve(struct MM_BaseMemory *base, u64 reserve_size) {
    struct Arena result = {};
    result.base = base;
    result.memory = base->reserve(base->ctx, reserve_size);
    result.cap = reserve_size;
    return (result); 
}

static struct Arena
mm_make_arena(struct MM_BaseMemory *base) {
    struct Arena result =  mm_make_arena_reserve(base, KB(64));
    return (result);
}

static struct 
Arena mm_scratch_arena() {
    // TODO CHANGE THIS TO GLOBAL
    struct Arena result = mm_make_arena_reserve(GLOBAL_BASE_ALLOCATOR, KB(64));
    return result;
}

static void 
mm_arena_release(struct Arena *arena) {
    struct MM_BaseMemory *base = arena->base;
    base->release(base->ctx, arena->memory, arena->cap);
} 

static void* 
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

static void* 
mm_arena_push_zeros(struct Arena *arena, u64 size) {
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
    mm_memset(result, 0, size);
    return (result);
}
