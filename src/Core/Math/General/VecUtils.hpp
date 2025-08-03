#pragma once

#include "MathUtils.hpp"
#include "Core/Math/Templates/Vec.hpp"
#include "Core/Math/Templates/Vec2.hpp"
#include "Core/Math/Templates/Vec3.hpp"
#include "Core/Math/Templates/Vec4.hpp"
#include "Core/Types/Defines.hpp"

namespace wfe {
	/// @brief A two-dimensional single precision floating point vector with all components set to zero.
	constexpr Vec2f VEC2F_ZERO { 0.0f, 0.0f };
	/// @brief A two-dimensional single precision floating point vector with all components set to one.
	constexpr Vec2f VEC2F_ONE { 1.0f, 1.0f };
	/// @brief A two-dimensional single precision floating point vector pointing right.
	constexpr Vec2f VEC2F_RIGHT { 1.0f, 0.0f };
	/// @brief A two-dimensional single precision floating point vector pointing left.
	constexpr Vec2f VEC2F_LEFT { -1.0f, 0.0f };
	/// @brief A two-dimensional single precision floating point vector pointing up.
	constexpr Vec2f VEC2F_UP { 0.0f, 1.0f };
	/// @brief A two-dimensional single precision floating point vector pointing down.
	constexpr Vec2f VEC2F_DOWN { 0.0f, -1.0f };

	/// @brief A two-dimensional double precision floating point vector with all components set to zero.
	constexpr Vec2d VEC2D_ZERO { 0.0, 0.0 };
	/// @brief A two-dimensional double precision floating point vector with all components set to one.
	constexpr Vec2d VEC2D_ONE { 1.0, 1.0 };
	/// @brief A two-dimensional double precision floating point vector pointing right.
	constexpr Vec2d VEC2D_RIGHT { 1.0, 0.0 };
	/// @brief A two-dimensional double precision floating point vector pointing left.
	constexpr Vec2d VEC2D_LEFT { -1.0, 0.0 };
	/// @brief A two-dimensional double precision floating point vector pointing up.
	constexpr Vec2d VEC2D_UP { 0.0, 1.0 };
	/// @brief A two-dimensional double precision floating point vector pointing down.
	constexpr Vec2d VEC2D_DOWN { 0.0, -1.0 };

	/// @brief A three-dimensional double precision floating point vector with all components set to zero.
	constexpr Vec3f VEC3F_ZERO { 0.0f, 0.0f, 0.0f };
	/// @brief A three-dimensional double precision floating point vector with all components set to one.
	constexpr Vec3f VEC3F_ONE { 1.0f, 1.0f, 1.0f };
	/// @brief A three-dimensional double precision floating point vector pointing right.
	constexpr Vec3f VEC3F_RIGHT { 1.0f, 0.0f, 0.0f };
	/// @brief A three-dimensional double precision floating point vector pointing left.
	constexpr Vec3f VEC3F_LEFT { -1.0f, 0.0f, 0.0f };
	/// @brief A three-dimensional double precision floating point vector pointing up.
	constexpr Vec3f VEC3F_UP { 0.0f, 1.0f, 0.0f };
	/// @brief A three-dimensional double precision floating point vector pointing down.
	constexpr Vec3f VEC3F_DOWN { 0.0f, -1.0f, 0.0f };
	/// @brief A three-dimensional double precision floating point vector pointing back.
	constexpr Vec3f VEC3F_BACK { 0.0f, 0.0f, 1.0f };
	/// @brief A three-dimensional double precision floating point vector pointing forward.
	constexpr Vec3f VEC3F_FORWARD { 0.0f, 0.0f, -1.0f };

	/// @brief A three-dimensional double precision floating point vector with all components set to zero.
	constexpr Vec3d VEC3D_ZERO { 0.0, 0.0, 0.0 };
	/// @brief A three-dimensional double precision floating point vector with all components set to one.
	constexpr Vec3d VEC3D_ONE { 1.0, 1.0, 1.0 };
	/// @brief A three-dimensional double precision floating point vector pointing right.
	constexpr Vec3d VEC3D_RIGHT { 1.0, 0.0, 0.0 };
	/// @brief A three-dimensional double precision floating point vector pointing left.
	constexpr Vec3d VEC3D_LEFT { -1.0, 0.0, 0.0 };
	/// @brief A three-dimensional double precision floating point vector pointing up.
	constexpr Vec3d VEC3D_UP { 0.0, 1.0, 0.0 };
	/// @brief A three-dimensional double precision floating point vector pointing down.
	constexpr Vec3d VEC3D_DOWN { 0.0, -1.0, 0.0 };
	/// @brief A three-dimensional double precision floating point vector pointing back.
	constexpr Vec3d VEC3D_BACK { 0.0, 0.0, 1.0 };
	/// @brief A three-dimensional double precision floating point vector pointing forward.
	constexpr Vec3d VEC3D_FORWARD { 0.0, 0.0, -1.0 };

	/// @brief A four-dimensional single precision floating point vector with all components set to zero.
	constexpr Vec4f VEC4F_ZERO { 0.0f, 0.0f, 0.0f, 0.0f };
	/// @brief A four-dimensional single precision floating point vector with all components set to one.
	constexpr Vec4f VEC4F_ONE { 1.0f, 1.0f, 1.0f, 1.0f };

	/// @brief A four-dimensional double precision floating point vector with all components set to zero.
	constexpr Vec4d VEC4D_ZERO { 0.0, 0.0, 0.0, 0.0 };
	/// @brief A four-dimensional double precision floating point vector with all components set to one.
	constexpr Vec4d VEC4D_ONE { 1.0, 1.0, 1.0, 1.0 };

	/// @brief Calculates the magnitude of the given vector.
	/// @param vec The vector whose magnitude to compute.
	/// @return The magnitude of the vector.
	inline float VecMagnitude(const Vec2f& vec) {
		return Sqrt(vec.x * vec.x + vec.y * vec.y);
	}
	/// @brief Calculates the magnitude of the given vector.
	/// @param vec The vector whose magnitude to compute.
	/// @return The magnitude of the vector.
	inline double VecMagnitude(const Vec2d& vec) {
		return Sqrt(vec.x * vec.x + vec.y * vec.y);
	}
	/// @brief Calculates the magnitude of the given vector.
	/// @param vec The vector whose magnitude to compute.
	/// @return The magnitude of the vector.
	inline float VecMagnitude(const Vec3f& vec) {
		return Sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}
	/// @brief Calculates the magnitude of the given vector.
	/// @param vec The vector whose magnitude to compute.
	/// @return The magnitude of the vector.
	inline double VecMagnitude(const Vec3d& vec) {
		return Sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}
	/// @brief Calculates the magnitude of the given vector.
	/// @param vec The vector whose magnitude to compute.
	/// @return The magnitude of the vector.
	inline float VecMagnitude(const Vec4f& vec) {
		return Sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
	}
	/// @brief Calculates the magnitude of the given vector.
	/// @param vec The vector whose magnitude to compute.
	/// @return The magnitude of the vector.
	inline double VecMagnitude(const Vec4d& vec) {
		return Sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
	}
	/// @brief Calculates the square of the magnitude of the given vector.
	/// @param vec The vector whose square of the magnitude to compute.
	/// @return The square of the magnitude of the vector.
	inline float VecSqrMagnitude(const Vec2f& vec) {
		return vec.x * vec.x + vec.y * vec.y;
	}
	/// @brief Calculates the square of the magnitude of the given vector.
	/// @param vec The vector whose square of the magnitude to compute.
	/// @return The square of the magnitude of the vector.
	inline double VecSqrMagnitude(const Vec2d& vec) {
		return vec.x * vec.x + vec.y * vec.y;
	}
	/// @brief Calculates the square of the magnitude of the given vector.
	/// @param vec The vector whose square of the magnitude to compute.
	/// @return The square of the magnitude of the vector.
	inline float VecSqrMagnitude(const Vec3f& vec) {
		return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	}
	/// @brief Calculates the square of the magnitude of the given vector.
	/// @param vec The vector whose square of the magnitude to compute.
	/// @return The square of the magnitude of the vector.
	inline double VecSqrMagnitude(const Vec3d& vec) {
		return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	}
	/// @brief Calculates the square of the magnitude of the given vector.
	/// @param vec The vector whose square of the magnitude to compute.
	/// @return The square of the magnitude of the vector.
	inline float VecSqrMagnitude(const Vec4f& vec) {
		return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
	}
	/// @brief Calculates the square of the magnitude of the given vector.
	/// @param vec The vector whose square of the magnitude to compute.
	/// @return The square of the magnitude of the vector.
	inline double VecSqrMagnitude(const Vec4d& vec) {
		return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
	}
	/// @brief Calculates the inverse of the magnitude of the given vector.
	/// @param vec The vector whose inverse of the magnitude to compute.
	/// @return The inverse of the magnitude of the vector.
	inline float VecInvMagnitude(const Vec2f& vec) {
		return InverseSqrt(vec.x * vec.x + vec.y * vec.y);
	}
	/// @brief Calculates the inverse of the magnitude of the given vector.
	/// @param vec The vector whose inverse of the magnitude to compute.
	/// @return The inverse of the magnitude of the vector.
	inline double VecInvMagnitude(const Vec2d& vec) {
		return InverseSqrt(vec.x * vec.x + vec.y * vec.y);
	}
	/// @brief Calculates the inverse of the magnitude of the given vector.
	/// @param vec The vector whose inverse of the magnitude to compute.
	/// @return The inverse of the magnitude of the vector.
	inline float VecInvMagnitude(const Vec3f& vec) {
		return InverseSqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}
	/// @brief Calculates the inverse of the magnitude of the given vector.
	/// @param vec The vector whose inverse of the magnitude to compute.
	/// @return The inverse of the magnitude of the vector.
	inline double VecInvMagnitude(const Vec3d& vec) {
		return InverseSqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}
	/// @brief Calculates the inverse of the magnitude of the given vector.
	/// @param vec The vector whose inverse of the magnitude to compute.
	/// @return The inverse of the magnitude of the vector.
	inline float VecInvMagnitude(const Vec4f& vec) {
		return InverseSqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
	}
	/// @brief Calculates the inverse of the magnitude of the given vector.
	/// @param vec The vector whose inverse of the magnitude to compute.
	/// @return The inverse of the magnitude of the vector.
	inline double VecInvMagnitude(const Vec4d& vec) {
		return InverseSqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
	}
	/// @brief Calculates the normalized version of the given vector.
	/// @param vec The vector to normalize.
	/// @return The normalized vector.
	inline Vec2f VecNormalized(const Vec2f& vec) {
		return vec * VecInvMagnitude(vec);
	}
	/// @brief Calculates the normalized version of the given vector.
	/// @param vec The vector to normalize.
	/// @return The normalized vector.
	inline Vec2d VecNormalized(const Vec2d& vec) {
		return vec * VecInvMagnitude(vec);
	}
	/// @brief Calculates the normalized version of the given vector.
	/// @param vec The vector to normalize.
	/// @return The normalized vector.
	inline Vec3f VecNormalized(const Vec3f& vec) {
		return vec * VecInvMagnitude(vec);
	}
	/// @brief Calculates the normalized version of the given vector.
	/// @param vec The vector to normalize.
	/// @return The normalized vector.
	inline Vec3d VecNormalized(const Vec3d& vec) {
		return vec * VecInvMagnitude(vec);
	}
	/// @brief Calculates the normalized version of the given vector.
	/// @param vec The vector to normalize.
	/// @return The normalized vector.
	inline Vec4f VecNormalized(const Vec4f& vec) {
		return vec * VecInvMagnitude(vec);
	}
	/// @brief Calculates the normalized version of the given vector.
	/// @param vec The vector to normalize.
	/// @return The normalized vector.
	inline Vec4d VecNormalized(const Vec4d& vec) {
		return vec * VecInvMagnitude(vec);
	}

	/// @brief Calculates the dot product of the given vectors.
	/// @param vec1 The first vector to multiply.
	/// @param vec2 The second vector to multiply.
	/// @return The dot product of the given vectors.
	inline float VecDot(const Vec2f& vec1, const Vec2f& vec2) {
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}
	/// @brief Calculates the dot product of the given vectors.
	/// @param vec1 The first vector to multiply.
	/// @param vec2 The second vector to multiply.
	/// @return The dot product of the given vectors.
	inline double VecDot(const Vec2d& vec1, const Vec2d& vec2) {
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}
	/// @brief Calculates the dot product of the given vectors.
	/// @param vec1 The first vector to multiply.
	/// @param vec2 The second vector to multiply.
	/// @return The dot product of the given vectors.
	inline float VecDot(const Vec3f& vec1, const Vec3f& vec2) {
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
	}
	/// @brief Calculates the dot product of the given vectors.
	/// @param vec1 The first vector to multiply.
	/// @param vec2 The second vector to multiply.
	/// @return The dot product of the given vectors.
	inline double VecDot(const Vec3d& vec1, const Vec3d& vec2) {
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
	}
	/// @brief Calculates the dot product of the given vectors.
	/// @param vec1 The first vector to multiply.
	/// @param vec2 The second vector to multiply.
	/// @return The dot product of the given vectors.
	inline float VecDot(const Vec4f& vec1, const Vec4f& vec2) {
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w;
	}
	/// @brief Calculates the dot product of the given vectors.
	/// @param vec1 The first vector to multiply.
	/// @param vec2 The second vector to multiply.
	/// @return The dot product of the given vectors.
	inline double VecDot(const Vec4d& vec1, const Vec4d& vec2) {
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w;
	}
	/// @brief Calculates the angle between the given vectors.
	/// @param vec1 The first vector.
	/// @param vec2 The second vector.
	/// @return The angle between the given vectors.
	inline float VecAngle(const Vec2f& vec1, const Vec2f& vec2) {
		return Acos(VecDot(vec1, vec2) * VecInvMagnitude(vec1) * VecInvMagnitude(vec2));
	}
	/// @brief Calculates the angle between the given vectors.
	/// @param vec1 The first vector.
	/// @param vec2 The second vector.
	/// @return The angle between the given vectors.
	inline double VecAngle(const Vec2d& vec1, const Vec2d& vec2) {
		return Acos(VecDot(vec1, vec2) * VecInvMagnitude(vec1) * VecInvMagnitude(vec2));
	}
	/// @brief Calculates the angle between the given vectors.
	/// @param vec1 The first vector.
	/// @param vec2 The second vector.
	/// @return The angle between the given vectors.
	inline float VecAngle(const Vec3f& vec1, const Vec3f& vec2) {
		return Acos(VecDot(vec1, vec2) * VecInvMagnitude(vec1) * VecInvMagnitude(vec2));
	}
	/// @brief Calculates the angle between the given vectors.
	/// @param vec1 The first vector.
	/// @param vec2 The second vector.
	/// @return The angle between the given vectors.
	inline double VecAngle(const Vec3d& vec1, const Vec3d& vec2) {
		return Acos(VecDot(vec1, vec2) * VecInvMagnitude(vec1) * VecInvMagnitude(vec2));
	}
	/// @brief Calculates the angle between the given vectors.
	/// @param vec1 The first vector.
	/// @param vec2 The second vector.
	/// @return The angle between the given vectors.
	inline float VecAngle(const Vec4f& vec1, const Vec4f& vec2) {
		return Acos(VecDot(vec1, vec2) * VecInvMagnitude(vec1) * VecInvMagnitude(vec2));
	}
	/// @brief Calculates the angle between the given vectors.
	/// @param vec1 The first vector.
	/// @param vec2 The second vector.
	/// @return The angle between the given vectors.
	inline double VecAngle(const Vec4d& vec1, const Vec4d& vec2) {
		return Acos(VecDot(vec1, vec2) * VecInvMagnitude(vec1) * VecInvMagnitude(vec2));
	}

	/// @brief Calculates the cross product of the given vectors.
	/// @param vec1 The first vector to multiply.
	/// @param vec2 The second vector to multiply.
	/// @return The resulting vector.
	inline Vec3f VecCross(const Vec3f& vec1, const Vec3f& vec2) {
		return {
			vec1.y * vec2.z - vec1.z * vec2.y,
			vec1.z * vec2.x - vec1.x * vec2.z,
			vec1.x * vec2.y - vec1.y * vec2.x
		};
	}
	/// @brief Calculates the cross product of the given vectors.
	/// @param vec1 The first vector to multiply.
	/// @param vec2 The second vector to multiply.
	/// @return The resulting vector.
	inline Vec3d VecCross(const Vec3d& vec1, const Vec3d& vec2) {
		return {
			vec1.y * vec2.z - vec1.z * vec2.y,
			vec1.z * vec2.x - vec1.x * vec2.z,
			vec1.x * vec2.y - vec1.y * vec2.x
		};
	}
}