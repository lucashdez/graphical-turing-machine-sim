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

// NOTE(lucashdez): HELPER MACROS
#define Statement(S) do {S}while(0)

#define Stringify_(S) #S
#define Stringify(S) Stringify_(S)

#define Glue_(A, B) A##B
#define Glue(A, B) Glue_(A, B)

#define ArrayCount(a) (sizeof(a)/sizeof(*(a)))


#endif //BASE_H