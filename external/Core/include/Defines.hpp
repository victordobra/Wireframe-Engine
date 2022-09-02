#pragma once

#include "BuildInfo.hpp"
#include <stdlib.h>
#include <assert.h>

namespace wfe {
    // Signed int types
    typedef ::int8_t  int8_t;
    typedef ::int16_t int16_t;
    typedef ::int32_t int32_t;
    typedef ::int64_t int64_t;

    // Unsigned int types
    typedef ::u_int8_t  uint8_t;
    typedef ::u_int16_t uint16_t;
    typedef ::u_int32_t uint32_t;
    typedef ::u_int64_t uint64_t;

    // Floating point types
    typedef float float32_t;
    typedef double float64_t;
        
    // Boolean types
    typedef bool bool8_t;
    typedef int bool32_t;

    // Character types
    typedef char char_t;
    typedef wchar_t charw_t;

#ifdef ARCH_X64
    typedef uint64_t size_t;
    typedef  int64_t ptrdiff_t;
    typedef  int64_t intptr_t;
#else
    typedef uint32_t size_t;
    typedef  int32_t ptrdiff_t;
    typedef  int32_t intptr_t;
#endif

    // Properly define static assertions
#if defined(__clang__) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif

    // Ensure all types are of the correct size
    STATIC_ASSERT((sizeof(int8_t)   == 1), "Expected int8_t to be 1 byte."   );
    STATIC_ASSERT((sizeof(int16_t)  == 2), "Expected int16_t to be 2 bytes." );
    STATIC_ASSERT((sizeof(int32_t)  == 4), "Expected int32_t to be 4 bytes." );
    STATIC_ASSERT((sizeof(int64_t)  == 8), "Expected int64_t to be 8 bytes." );

    STATIC_ASSERT((sizeof(uint8_t)   == 1), "Expected uint8_t to be 1 byte."   );
    STATIC_ASSERT((sizeof(uint16_t)  == 2), "Expected uint16_t to be 2 bytes." );
    STATIC_ASSERT((sizeof(uint32_t)  == 4), "Expected uint32_t to be 4 bytes." );
    STATIC_ASSERT((sizeof(uint64_t)  == 8), "Expected uint64_t to be 8 bytes." );

    STATIC_ASSERT((sizeof(float32_t) == 4), "Expected float32_t to be 4 bytes.");
    STATIC_ASSERT((sizeof(float64_t) == 8), "Expected float64_t to be 8 bytes.");

    STATIC_ASSERT((sizeof(bool8_t)   == 1), "Expected bool8_t to be 1 byte."   );
    STATIC_ASSERT((sizeof(bool32_t)  == 4), "Expected bool32_t to be 4 bytes." );

#define TRUE 1
#define FALSE 0

    // Inlining
#ifdef _MSC_VER
#define INLINE __forceinline
#define NOINLINE __declspec(noinline)
#else
#define INLINE static inline
#define NOINLINE
#endif
}