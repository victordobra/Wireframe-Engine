#pragma once

#include "Core.hpp"
#include "Matrix4x4.hpp"
#include "Colors.hpp"
#include <math.h>
#include <time.h>

namespace wfe {
	constexpr float32_t E = 2.71828f;

	constexpr float32_t PI = 3.14159265358979323846f;
	constexpr float32_t PI_2 = PI * 2.f;
	constexpr float32_t HALF_PI = PI * 0.5f;
	constexpr float32_t QUARTER_PI = PI * 0.25f;
	constexpr float32_t INV_PI = 1.f / PI;
	constexpr float32_t INV_PI_2 = 1.f / PI_2;

	constexpr float32_t SQRT_TWO = 1.41421356237309504880f;
	constexpr float32_t SQRT_THREE = 1.73205080756887729352f;
	constexpr float32_t SQRT_INV_TWO = 0.70710678118654752440f;
	constexpr float32_t SQRT_INV_THREE = 0.57735026918962576450f;

	constexpr float32_t DEG_TO_RAD_MULTIPLIER = PI / 180.f;
	constexpr float32_t RAD_TO_DEG_MULTIPLIER = 180.f / PI;

	constexpr float32_t SEC_TO_MS_MULTIPLIER = 1000.f;
	constexpr float32_t MS_TO_SEC_MULTIPLIER = 0.001f;

	constexpr float32_t INFINITY_VAL = 1e30f;
	constexpr float32_t FLOAT_EPSILON = 1.192092896e-07f;

	inline float32_t Sin(float32_t x) { return sinf(x); }
	inline float32_t Cos(float32_t x) { return cosf(x); }
	inline float32_t Tan(float32_t x) { return tanf(x); }
	inline float32_t Asin(float32_t x) { return asinf(x); }
	inline float32_t Acos(float32_t x) { return acosf(x); }
	inline float32_t Atan(float32_t x) { return atanf(x); }
	inline float32_t Atan2(float32_t x, float32_t y) { return atan2f(x, y); }

	inline float32_t Sqrt(float32_t x) { return sqrtf(x); }

	inline bool8_t IsPowerOf2(uint64_t value) {
		return (value != 0) && ((value & (value - 1)) == 0); // Based and redpilled epic gayer bithacking
	}

	inline sint32_t Random(uint32_t seed = _time32(0)) {
		srand(seed);
		return rand();
	}
	inline sint32_t RandomInRange(sint32_t min, sint32_t max, uint32_t seed = _time32(0)) {
		srand(seed);

		assert((max >= min) && "The maximum value must be higher than or equal to the minimum value!");

		sint32_t dif = max - min;
		sint32_t num = rand() % dif + min;

		return num;
	}

	inline float32_t RandomF(uint32_t seed = _time32(0)) {
		srand(seed);
		return (float32_t)rand();
	}
	inline float32_t RandomInRangeF(float32_t min, float32_t max, uint32_t seed = _time32(0)) {
		srand(seed);

		assert((max >= min) && "The maximum value must be higher than or equal to the minimum value!");

		float32_t dif = max - min;
		float32_t num = (float32_t)rand() / (float32_t)(RAND_MAX / dif) + min;

		return num;
	}

	template<class T>
	inline T Lerp(T x, T y, float32_t t) { return x + (x - y) * t; }
}