/* date = June 3rd 2024 2:07 pm */


#ifndef BASE_H
#define BASE_H

#define function static
#define internal static
#define local_persist static
#define global static


typedef unsigned char u8;
typedef signed   char i8;
typedef unsigned short u16;
typedef signed   short i16;
typedef unsigned int u32;
typedef signed   int i32;
typedef unsigned long long u64;
typedef signed   long long i64;
typedef float  f32;
typedef double f64;

// Booleans
typedef i8 b8;
typedef i32 b32; 
typedef i64 b64;
#define false 0
#define true !false 

// Sized Strings
typedef struct String_Const_u8 {
    u8 *str;
    u64 size;
} String8;

struct String_Const_Char {
    char* str;
    u64 size;
};  


///////////////////////////////////
// HELPER MACROS
#define Statement(S) do {S}while(0)

// Strings
#define Stringify_(S) #S
#define Stringify(S) Stringify_(S)

#define Glue_(A, B) A##B
#define Glue(A, B) Glue_(A, B)
#define GlueStr(Astr, Bstr) Astr Bstr

#define string_u8_litexpr(S) (String8) {(u8*)S, sizeof(S)/sizeof(u8)}

// Math
#define Min(a, b) a<b?a:b
#define Max(a, b) a>b?a:b
#define clampTop(a, b) Min(a, b)
#define clampBot(a, b) Max(a, b)

// Arrays
#define ArrayCount(a) (sizeof(a)/sizeof(*(a)))

// Memory types

#define BYTE(x) (x)
#define KB(x) ((x << 10))
#define MB(x) ((x << 20))
#define MM_COMMIT_BLOCK_SIZE MB(64)

#define align2pow(x, p) (((x) + (p) - 1) &~ ((p) - 1))

// Double Linked_List
//   PushBack

//   PushFront
//   PopFront
//   PopBack

// Simple Linked-List Queue

// Simple Linked-List Stack

#endif //BASE_H