#pragma once
#include <cstring>
#include <cstdlib>

namespace mge {

//Signed int types
typedef signed char sint8_t;
typedef signed short sint16_t;
typedef signed long sint32_t;
typedef signed long long sint64_t;

//Unsigned int types
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;

//Floating point types
typedef float float32_t;
typedef double float64_t;
	
//Boolean types
typedef bool bool8_t;
typedef int bool32_t;

#ifdef _WIN64
typedef uint64_t size_t;
#else
typedef uint32_t size_t;
#endif

//Properly define static assertions
#if defined(__clang__) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif

//Ensure all types are of the correct size
STATIC_ASSERT(sizeof(sint8_t) == 1, "Expected sint8_t to be 1 byte.");
STATIC_ASSERT(sizeof(sint16_t) == 2, "Expected sint16_t to be 2 bytes.");
STATIC_ASSERT(sizeof(sint32_t) == 4, "Expected sint32_t to be 4 bytes.");
STATIC_ASSERT(sizeof(sint64_t) == 8, "Expected sint64_t to be 8 bytes.");

STATIC_ASSERT(sizeof(uint8_t) == 1, "Expected uint8_t to be 1 byte.");
STATIC_ASSERT(sizeof(uint16_t) == 2, "Expected uint16_t to be 2 bytes.");
STATIC_ASSERT(sizeof(uint32_t) == 4, "Expected uint32_t to be 4 bytes.");
STATIC_ASSERT(sizeof(uint64_t) == 8, "Expected uint64_t to be 8 bytes.");

STATIC_ASSERT(sizeof(float32_t) == 4, "Expected float32_t to be 4 bytes.");
STATIC_ASSERT(sizeof(float64_t) == 8, "Expected float64_t to be 8 bytes.");

STATIC_ASSERT(sizeof(bool8_t) == 1, "Expected bool8_t to be 1 byte.");
STATIC_ASSERT(sizeof(bool32_t) == 4, "Expected bool32_t to be 4 bytes.");

#define TRUE 1
#define FALSE 0

//Inlining
#ifdef _MSC_VER
#define INLINE __forceinline
#define NOINLINE __declspec(noinline)
#else
#define INLINE static inline
#define NOINLINE
#endif
}