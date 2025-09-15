/* date = June 3rd 2024 2:07 pm */
#ifndef BASE_H
#define BASE_H

#define VIRTUAL_WIDTH 1920
#define VIRTUAL_HEIGHT 1080

/*~ base-types */

#define local_persist static
#define global_var static
#define internal static

#  define ColorDebug "\x1b[38;2;105;105;105m"
#  define ColorInfo "\x1b[38;2;7;228;218m"
#  define ColorWarn "\x1b[38;2;255;182;11m"
#  define ColorError "\x1b[38;2;255;59;59m"
#  define ColorReset  "\x1b[0m"

typedef unsigned char u8;
typedef signed   char s8;
typedef unsigned short u16;
typedef signed   short s16;
typedef unsigned int u32;
typedef signed   int s32;
typedef unsigned long long u64;
typedef signed   long long s64;
typedef float  f32;
typedef double f64;

/* Booleans */
typedef s32 b32; 

#define false 0
#define true !false 

/* Sized Strings */
typedef struct StringConstU8 {
    const u8 *str;
    u64 size;
} StringU8;

typedef struct StringConstChar {
    const char* str;
    u64 size;
} StringChar;  


/* Math */
typedef struct Vec2s32 {
	s32 x;
	s32 y;

} Vec2s32, Vec2;

typedef union Rects32 {
	struct {
		s32 x1, y1, x2, y2;
	};

	struct {
		Vec2s32 p1, p2, p3, p4;
	};

	struct {
		Vec2s32 p;
		s32 width, height;
	};
	
} Rects32;



/* HELPER MACROS */

#define ERR(S, ...) printf(ColorError   "[ERROR] %s:%d -> " S ColorReset "\n",__FILE__, __LINE__, __VA_ARGS__)
#define WARN(S, ...) printf(ColorWarn   "[WARN]  %s:%d -> " S ColorReset "\n",__FILE__, __LINE__, __VA_ARGS__)
#define DEBUG(S, ...) printf(ColorDebug "[DEBUG] %s:%d -> " S ColorReset "\n",__FILE__, __LINE__, __VA_ARGS__)
#define INFO(S, ...) printf(ColorInfo   "[INFO]  %s:%d -> " S ColorReset "\n",__FILE__, __LINE__, __VA_ARGS__)

#define ERRMSG(S, ...)   printf(ColorError "[ERROR] %s:%d -> " S ColorReset "\n",__FILE__, __LINE__)
#define WARNMSG(S, ...)  printf(ColorWarn  "[WARN]  %s:%d -> " S ColorReset "\n",__FILE__, __LINE__)
#define DEBUGMSG(S, ...) printf(ColorDebug "[DEBUG] %s:%d -> " S ColorReset "\n",__FILE__, __LINE__)
#define INFOMSG(S, ...)  printf(ColorInfo  "[INFO]  %s:%d -> " S ColorReset "\n",__FILE__, __LINE__)


#define Statement(S) do {S}while(0)

/* Strings */
#define Stringify_(S) #S
#define Stringify(S) Stringify_(S)

#define Glue_(A, B) A##B
#define Glue(A, B) Glue_(A, B)
#define GlueStr(Astr, Bstr) Astr Bstr


#define string_u8_litexpr(S) (String8) {(u8*)S, sizeof(S)/sizeof(u8) - 1}
#define s8Lit(S) string_u8_litexpr(S)

/* Math */
#define Min(a, b) a<b?a:b
#define Max(a, b) a>b?a:b
#define clampTop(a, b) Min(a, b)
#define clampBot(a, b) Max(a, b)

/* Arrays */
#define ArrayCount(a) (sizeof(a)/sizeof(*(a)))

/* Memory */
#define KB(x) ((x << 10))
#define MB(x) ((x << 20))
#define MM_COMMIT_BLOCK_SIZE MB(64)

#define align2pow(x, p) (((x) + (p) - 1) &~ ((p) - 1))

#define MMPushArrayZeros(a, T, c) (T*)mm_arena_push_zeros(a, sizeof(T)*c)
#define MMPushArray(a, T, c) (T*)mm_arena_push(a, sizeof(T)*c)

/* Strings */
#define MMPushString8(a, s) mm_arena_push(a, s.size)
#define MMPushString(a, s) mm_arena_push(a, sizeof(*s)*sizeof(s))

/* TODO Double Linked_List */
/* PushBack */
/*   PushFront */
/*   PopFront */
/*   PopBack */

/* Simple Linked-List Queue */
/*   TODO Push */
#define sll_queue_push_multiple_(f, l, ff, ll) if(ll){if(f){l->next=ff;}else{f=ff;}l=ll;l->next=0;}
#define sll_queue_push_(f, l, n) sll_queue_push_multiple_(f,l,n,n)
#define sll_queue_push(f, l, n) Statement(sll_queue_push_((f), (l), (n)))
/* TODO Pop */
#define sll_queue_pop_(f, l) if(f==l){f=l=0;} else {f=f->next;}
#define sll_queue_pop(f, l) Statement(sll_queue_pop_((f), (l)))

/* Simple Linked-List Stack */
#define sll_stack_push(h, n) n->next = h, h=n
#define sll_stack_pop(h) h = h->next 


/*~ MEMORY */
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
    u8* memory; /* pointer to the initial */
    u64 cap;
    u64 pos;
    u64 commit_pos;
} Arena;

/* UTILITIES */
internal void* mm_memset(void* dest, int val, u64 len);
internal void* mm_memcpy(void* dest, void* src, u64 len);
internal void* mm_memcpymv(void* dest, void* src, u64 len);

internal struct MM_BaseMemory* mm_create_malloc_base_memory(void);
internal void mm_change_memory_noop(void* ctx, void* ptr, u64 size) {}
internal Arena mm_make_arena_reserve(MM_BaseMemory *base, u64 reserve_size);
internal Arena mm_make_arena(MM_BaseMemory *base);
internal Arena mm_scratch_arena();


internal void mm_arena_release(Arena *arena);
internal void* mm_arena_push(Arena *arena, u64 size);
internal void* mm_arena_push_zeros(Arena *arena, u64 size);
internal void mm_arena_pop_to(Arena *arena, u64 pos);


/* GLOBALS */
global_var MM_BaseMemory* GLOBAL_BASE_ALLOCATOR = {0};




#endif /* BASE_H_ */
