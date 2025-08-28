#pragma once

#include "Core/Types/Defines.hpp"
#include <cmath>

namespace wfe {
	/// @brief Single precision floating point constant for pi.
	constexpr float PI_F = 3.14159265358979323846f;
	/// @brief Single precision floating point constant for half of pi.
	constexpr float HALF_PI_F = 1.57079632679489661923f;
	/// @brief Single precision floating point constant for a quarter of pi.
	constexpr float QUARTER_PI_F = 0.785398163397448309616f;
	/// @brief Single precision floating point multiplier for conversion from degrees to radians.
	constexpr float DEG_TO_RAD_F = 0.017453292519943295769f;
	/// @brief Single precision floating point multiplier for conversion from radians to degrees.
	constexpr float RAD_TO_DEG_F = 57.295779513082320876846f;

	/// @brief Double precision floating point constant for pi.
	constexpr double PI_D = 3.14159265358979323846;
	/// @brief Double precision floating point constant for half of pi.
	constexpr double HALF_PI_D = 1.57079632679489661923;
	/// @brief Double precision floating point constant for a quarter of pi.
	constexpr double QUARTER_PI_D = 0.785398163397448309616;
	/// @brief Double precision floating point multiplier for conversion from degrees to radians.
	constexpr double DEG_TO_RAD_D = 0.017453292519943295769;
	/// @brief Double precision floating point multiplier for conversion from radians to degrees.
	constexpr double RAD_TO_DEG_D = 57.295779513082320876846;

	/// @brief Gets the minimum of the two given values.
	/// @tparam T The type of the values to compare.
	/// @param val1 The first value to compare.
	/// @param val2 The second value to compare.
	/// @return The minimum of the two given values.
	template<class T>
	inline const T& Min(const T& val1, const T& val2) {
		if(val1 < val2) {
			return val1;
		} else {
			return val2;
		}
	}
	/// @brief Gets the maximum of the two given values.
	/// @tparam T The type of the values to compare.
	/// @param val1 The first value to compare.
	/// @param val2 The second value to compare.
	/// @return The maximum of the two given values.
	template<class T>
	inline const T& Max(const T& val1, const T& val2) {
		if(val1 > val2) {
			return val1;
		} else {
			return val2;
		}
	}
	/// @brief Gets the absolute value of the given numerical value.
	/// @tparam T The value type to check.
	/// @param val The value to check.
	/// @return The absolute value.
	template<class T>
	inline T Abs(const T& val) {
		if(val < 0) {
			return -val;
		} else {
			return val;
		}
	}

	/// @brief Gets the square root of the given value.
	/// @param val The value to get the square root of.
	/// @return The square root of the value.
	inline float Sqrt(float val) {
		return std::sqrtf(val);
	}
	/// @brief Gets the square root of the given value.
	/// @param val The value to get the square root of.
	/// @return The square root of the value.
	inline double Sqrt(double val) {
		return std::sqrt(val);
	}
	/// @brief Gets the inverse square root of the given value.
	/// @param val The value to get the inverse square root of.
	/// @return The inverse square root of the value.
	inline float InverseSqrt(float val) {
		return 1 / Sqrt(val);
	}
	/// @brief Gets the inverse square root of the given value.
	/// @param val The value to get the inverse square root of.
	/// @return The inverse square root of the value.
	inline double InverseSqrt(double val) {
		return 1 / Sqrt(val);
	}
	/// @brief Raises a value to the given exponent.
	/// @param base The value to raise.
	/// @param exp The exponent to raise the value to.
	/// @return The result of the operation.
	inline float Pow(float base, float exp) {
		return std::powf(base, exp);
	}
	/// @brief Raises a value to the given exponent.
	/// @param base The value to raise.
	/// @param exp The exponent to raise the value to.
	/// @return The result of the operation.
	inline double Pow(double base, double exp) {
		return std::pow(base, exp);
	}

	/// @brief Computes the sine of the given value.
	/// @param val The value to use.
	/// @return The sine of the value.
	inline float Sin(float val) {
		return std::sinf(val);
	}
	/// @brief Computes the sine of the given value.
	/// @param val The value to use.
	/// @return The sine of the value.
	inline double Sin(double val) {
		return std::sin(val);
	}
	/// @brief Computes the cosine of the given value.
	/// @param val The value to use.
	/// @return The cosine of the value.
	inline float Cos(float val) {
		return std::cosf(val);
	}
	/// @brief Computes the cosine of the given value.
	/// @param val The value to use.
	/// @return The cosine of the value.
	inline double Cos(double val) {
		return std::cos(val);
	}
	/// @brief Computes the tangent of the given value.
	/// @param val The value to use.
	/// @return The tangent of the value.
	inline float Tan(float val) {
		return std::tanf(val);
	}
	/// @brief Computes the tangent of the given value.
	/// @param val The value to use.
	/// @return The tangent of the value.
	inline double Tan(double val) {
		return std::tan(val);
	}
	/// @brief Computes the arc sine of the given value.
	/// @param val The value to use.
	/// @return The arc sine of the value.
	inline float Asin(float val) {
		return std::asinf(val);
	}
	/// @brief Computes the arc sine of the given value.
	/// @param val The value to use.
	/// @return The arc sine of the value.
	inline double Asin(double val) {
		return std::asin(val);
	}
	/// @brief Computes the arc cosine of the given value.
	/// @param val The value to use.
	/// @return The arc cosine of the value.
	inline float Acos(float val) {
		return std::acosf(val);
	}
	/// @brief Computes the arc cosine of the given value.
	/// @param val The value to use.
	/// @return The arc cosine of the value.
	inline double Acos(double val) {
		return std::acos(val);
	}
	/// @brief Computes the arc tangent of the given value.
	/// @param val The value to use.
	/// @return The arc tangent of the value.
	inline float Atan(float val) {
		return std::atanf(val);
	}
	/// @brief Computes the arc tangent of the given value.
	/// @param val The value to use.
	/// @return The arc tangent of the value.
	inline double Atan(double val) {
		return std::atan(val);
	}
	/// @brief Computes the arc tangent for the two given coordinates.
	/// @param y The Y coordinate.
	/// @param x The X coordinate.
	/// @return The arc tangent for the two coordinates.
	inline float Atan2(float y, float x) {
		return std::atan2f(y, x);
	}
	/// @brief Computes the arc tangent for the two given coordinates.
	/// @param y The Y coordinate.
	/// @param x The X coordinate.
	/// @return The arc tangent for the two coordinates.
	inline double Atan2(double y, double x) {
		return std::atan2(y, x);
	}
}
