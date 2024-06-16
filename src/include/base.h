/* date = June 3rd 2024 2:07 pm */

#ifndef BASE_H
#define BASE_H

#define function static
#define local static
#define global static

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef int i16;
typedef long int i32;
typedef long long i64;

typedef float f32;
typedef double f64;

///////////////////////////////////
// HELPER MACROS
#define Statement(S) do {S}while(0)

// Strings
#define Stringify_(S) #S
#define Stringify(S) Stringify_(S)

#define Glue_(A, B) A##B
#define Glue(A, B) Glue_(A, B)

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