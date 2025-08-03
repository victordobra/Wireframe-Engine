#pragma once

#include "MathUtils.hpp"
#include "Core/Math/Templates/Quat.hpp"
#include "Core/Math/Templates/Vec3.hpp"
#include "Core/Types/Defines.hpp"

namespace wfe {
	/// @brief A single precision floating point quaternion representing no spatial rotation.
	constexpr Quatf QUATF_IDENTITY { 0.0f, 0.0f, 0.0f, 1.0f };
	/// @brief A double precision floating point quaternion representing no spatial rotation.
	constexpr Quatd QUATD_IDENTITY { 0.0, 0.0, 0.0, 1.0 };

	/// @brief Calculates the magnitude of the given quaternion.
	/// @param quat The quaternion whose magnitude to compute.
	/// @return The magnitude of the quaternion.
	inline float QuatMagnitude(const Quatf& quat) {
		return Sqrt(quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w);
	}
	/// @brief Calculates the magnitude of the given quaternion.
	/// @param quat The quaternion whose magnitude to compute.
	/// @return The magnitude of the quaternion.
	inline double QuatMagnitude(const Quatd& quat) {
		return Sqrt(quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w);
	}
	/// @brief Calculates the square of the magnitude of the given quaternion.
	/// @param quat The quaternion whose square of the magnitude to compute.
	/// @return The square of the magnitude of the quaternion.
	inline float QuatSqrMagnitude(const Quatf& quat) {
		return quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w;
	}
	/// @brief Calculates the square of the magnitude of the given quaternion.
	/// @param quat The quaternion whose square of the magnitude to compute.
	/// @return The square of the magnitude of the quaternion.
	inline double QuatSqrMagnitude(const Quatd& quat) {
		return quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w;
	}
	/// @brief Calculates the inverse of the magnitude of the given quaternion.
	/// @param quat The quaternion whose inverse of the magnitude to compute.
	/// @return The inverse of the magnitude of the quaternion.
	inline float QuatInvMagnitude(const Quatf& quat) {
		return InverseSqrt(quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w);
	}
	/// @brief Calculates the inverse of the magnitude of the given quaternion.
	/// @param quat The quaternion whose inverse of the magnitude to compute.
	/// @return The inverse of the magnitude of the quaternion.
	inline double QuatInvMagnitude(const Quatd& quat) {
		return InverseSqrt(quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w);
	}
	/// @brief Calculates the normalized version of the given quaternion.
	/// @param quat The quaternion to normalize.
	/// @return The normalized quaternion.
	inline Quatf QuatNormalized(const Quatf& quat) {
		return quat * QuatInvMagnitude(quat);
	}
	/// @brief Calculates the normalized version of the given quaternion.
	/// @param quat The quaternion to normalize.
	/// @return The normalized quaternion.
	inline Quatd QuatNormalized(const Quatd& quat) {
		return quat * QuatInvMagnitude(quat);
	}

	/// @brief Gets the conjugate of the given quaternion.
	/// @param quat The quaternion whose conjugate to get.
	/// @return The quaternion's conjugate.
	inline Quatf QuatConjugate(const Quatf& quat) {
		return { -quat.x, -quat.y, -quat.z, quat.w };
	}
	/// @brief Gets the conjugate of the given quaternion.
	/// @param quat The quaternion whose conjugate to get.
	/// @return The quaternion's conjugate.
	inline Quatd QuatConjugate(const Quatd& quat) {
		return { -quat.x, -quat.y, -quat.z, quat.w };
	}
	/// @brief Gets the inverse of the given quaternion.
	/// @param quat The quaternion whose inverse to get.
	/// @return The quaternion's inverse.
	inline Quatf QuatInverse(const Quatf& quat) {
		return QuatConjugate(quat) / QuatSqrMagnitude(quat);
	}
	/// @brief Gets the inverse of the given quaternion.
	/// @param quat The quaternion whose inverse to get.
	/// @return The quaternion's inverse.
	inline Quatd QuatInverse(const Quatd& quat) {
		return QuatConjugate(quat) / QuatSqrMagnitude(quat);
	}

	/// @brief Builds a unit quaternion representing rotation around an axis.
	/// @param angle The angle, in radians, of the rotation.
	/// @param axis The axis of the rotation. Must be an unit vector.
	/// @return The resulting quaternion.
	inline Quatf QuatRotAroundAxis(float angle, const Vec3f& axis) {
		// Calculate the sine and cosine of half of the given angle
		float halfSin = Sin(angle * 0.5f);
		float halfCos = Cos(angle * 0.5f);

		// Build the unit rotation quaternion
		return { axis.x * halfSin, axis.y * halfSin, axis.z * halfSin, halfCos };
	}
}