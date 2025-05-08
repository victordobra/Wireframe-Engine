#pragma once

#include "BuildInfo.hpp"
#include <stdint.h>
#include <assert.h>
#include <limits.h>
#include <atomic>

namespace wfe {
	// Signed int types
	typedef ::int8_t  int8_t;
	typedef ::int16_t int16_t;
	typedef ::int32_t int32_t;
	typedef ::int64_t int64_t;

	// Unsigned int types
	typedef ::uint8_t  uint8_t;
	typedef ::uint16_t uint16_t;
	typedef ::uint32_t uint32_t;
	typedef ::uint64_t uint64_t;

	// Floating point types
	typedef float  float32_t;
	typedef double float64_t;
		
	// Architecture specific types
#ifdef WFE_ARCHITECTURE_64_BIT
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
#define WFE_STATIC_ASSERT _Static_assert
#else
#define WFE_STATIC_ASSERT static_assert
#endif

	// Ensure all types are of the correct size
	WFE_STATIC_ASSERT((sizeof(int8_t)   == 1), "Expected int8_t to be 1 byte!"  );
	WFE_STATIC_ASSERT((sizeof(int16_t)  == 2), "Expected int16_t to be 2 bytes!");
	WFE_STATIC_ASSERT((sizeof(int32_t)  == 4), "Expected int32_t to be 4 bytes!");
	WFE_STATIC_ASSERT((sizeof(int64_t)  == 8), "Expected int64_t to be 8 bytes!");

	WFE_STATIC_ASSERT((sizeof(uint8_t)   == 1), "Expected uint8_t to be 1 byte!"  );
	WFE_STATIC_ASSERT((sizeof(uint16_t)  == 2), "Expected uint16_t to be 2 bytes!");
	WFE_STATIC_ASSERT((sizeof(uint32_t)  == 4), "Expected uint32_t to be 4 bytes!");
	WFE_STATIC_ASSERT((sizeof(uint64_t)  == 8), "Expected uint64_t to be 8 bytes!");

	WFE_STATIC_ASSERT((sizeof(float32_t) == 4), "Expected float32_t to be 4 bytes!");
	WFE_STATIC_ASSERT((sizeof(float64_t) == 8), "Expected float64_t to be 8 bytes!");

	// Atomic types
	typedef std::atomic<int8_t>  atomic_int8_t;
	typedef std::atomic<int16_t> atomic_int16_t;
	typedef std::atomic<int32_t> atomic_int32_t;
	typedef std::atomic<int64_t> atomic_int64_t;

	typedef std::atomic<uint8_t>  atomic_uint8_t;
	typedef std::atomic<uint16_t> atomic_uint16_t;
	typedef std::atomic<uint32_t> atomic_uint32_t;
	typedef std::atomic<uint64_t> atomic_uint64_t;

	typedef std::atomic<float32_t> atomic_float32_t;
	typedef std::atomic<float64_t> atomic_float64_t;

	typedef std::atomic<size_t> atomic_size_t;
	typedef std::atomic<ptrdiff_t> atomic_ptrdiff_t;
	typedef std::atomic<intptr_t> atomic_intptr_t;

	// Numerical limits for types

	/// @brief The minimum value an int8_t can hold.
	constexpr int64_t INT8_T_MIN = -128;
	/// @brief The maximum value an int8_t can hold.
	constexpr int64_t INT8_T_MAX = 127;
	/// @brief The minimum value an int16_t can hold.
	constexpr int64_t INT16_T_MIN = -32768;
	/// @brief The maximum value an int16_t can hold.
	constexpr int64_t INT16_T_MAX = 32767;
	/// @brief The minimum value an int32_t can hold.
	constexpr int64_t INT32_T_MIN = -2147483648;
	/// @brief The maximum value an int32_t can hold.
	constexpr int64_t INT32_T_MAX = 2147483647;
	/// @brief The minimum value an int64_t can hold.
	constexpr int64_t INT64_T_MIN = -9223372036854775807 - 1;
	/// @brief The maximum value an int64_t can hold.
	constexpr int64_t INT64_T_MAX = 9223372036854775807;

	/// @brief The minimum value an uint8_t can hold.
	constexpr uint64_t UINT8_T_MIN = 0;
	/// @brief The maximum value an uint8_t can hold.
	constexpr uint64_t UINT8_T_MAX = 255;
	/// @brief The minimum value an uint16_t can hold.
	constexpr uint64_t UINT16_T_MIN = 0;
	/// @brief The maximum value an uint16_t can hold.
	constexpr uint64_t UINT16_T_MAX = 65536;
	/// @brief The minimum value an uint32_t can hold.
	constexpr uint64_t UINT32_T_MIN = 0;
	/// @brief The maximum value an uint32_t can hold.
	constexpr uint64_t UINT32_T_MAX = 4294967295;
	/// @brief The minimum value an uint64_t can hold.
	constexpr uint64_t UINT64_T_MIN = 0;
	/// @brief The maximum value an uint64_t can hold.
	constexpr uint64_t UINT64_T_MAX = 18446744073709551615ULL;

	/// @brief The minimum absolute value a float32_t can hold.
	constexpr float32_t FLOAT32_T_MIN_ABS = 1.17549e-38;
	/// @brief The minimum value a float32_t can hold.
	constexpr float32_t FLOAT32_T_MIN = -3.40282e+38;
	/// @brief The maximum value a float32_t can hold.
	constexpr float32_t FLOAT32_T_MAX = 3.40282e+38;

	/// @brief The minimum absolute value a float64_t can hold.
	constexpr float64_t FLOAT64_T_MIN_ABS = 2.22507e-308;
	/// @brief The minimum value a float64_t can hold.
	constexpr float64_t FLOAT64_T_MIN = -1.79769e+308;
	/// @brief The maximum value a float64_t can hold.
	constexpr float64_t FLOAT64_T_MAX = 1.79769e+308;

#if defined(WFE_ARCHITECTURE_64_BIT)
	/// @brief The minimim value a size_t can hold.
	constexpr uint64_t SIZE_T_MIN = UINT64_T_MIN;
	/// @brief The maximim value a size_t can hold.
	constexpr uint64_t SIZE_T_MAX = UINT64_T_MAX;
	/// @brief The minimim value a ptrdiff_t can hold.
	constexpr int64_t PTRDIFF_T_MIN = INT64_T_MIN;
	/// @brief The maximim value a ptrdiff_t can hold.
	constexpr int64_t PTRDIFF_T_MAX = INT64_T_MAX;
	/// @brief The minimim value an intptr_t can hold.
	constexpr int64_t INTPTR_T_MIN = INT64_T_MIN;
	/// @brief The maximim value an intptr_t can hold.
	constexpr int64_t INTPTR_T_MAX = INT64_T_MAX;
#else
	/// @brief The minimim value a size_t can hold.
	constexpr uint64_t SIZE_T_MIN = UINT32_T_MIN;
	/// @brief The maximim value a size_t can hold.
	constexpr uint64_t SIZE_T_MAX = UINT32_T_MAX;
	/// @brief The minimim value a ptrdiff_t can hold.
	constexpr int64_t PTRDIFF_T_MIN = INT32_T_MIN;
	/// @brief The maximim value a ptrdiff_t can hold.
	constexpr int64_t PTRDIFF_T_MAX = INT32_T_MAX;
	/// @brief The minimim value an intptr_t can hold.
	constexpr int64_t INTPTR_T_MIN = INT32_T_MIN;
	/// @brief The maximim value an intptr_t can hold.
	constexpr int64_t INTPTR_T_MAX = INT32_T_MAX;
#endif

/// @brief Gets the string version of the macro's value. Used for a string conversion workaround.
/// @param macro The macro whose string value to return.
#define WFE_MACRO_STRING(macro) #macro
/// @brief Concatenates the two given macros. Used for a macro value conversion workaround.
/// @param macro1 The first macro to concatenate.
/// @param macro2 The second macro to concatenate.
#define WFE_MACRO_CONCAT(macro1, macro2) macro1 ## macro2
}