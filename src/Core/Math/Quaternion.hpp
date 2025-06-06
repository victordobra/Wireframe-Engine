#pragma once

#include "Core/Types/Defines.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace wfe {
	/// @brief A class that holds a quaternion. Used for representing 3D rotations.
	class Quaternion {
	public:
		/// @brief An identity quaternion, representing no rotation.
		static const Quaternion IDENTITY;

		union {
			/// @brief An array containing all of the quaternion's elements.
			float elements[4];

			struct {
				/// @brief The X dimension of the quaternion.
				float x;
				/// @brief The Y dimension of the quaternion.
				float y;
				/// @brief The Z dimension of the quaternion.
				float z;
				/// @brief The W dimension of the quaternion.
				float w;
			};
		};

		/// @brief Creates an identity quaternion.
		Quaternion();
		/// @brief Copies the given quaternion.
		/// @param other The quaternion to copy.
		Quaternion(const Quaternion& other) = default;
		/// @brief Moves the given quaternion's contents.
		/// @param other The quaternion to move.
		Quaternion(Quaternion&& other) noexcept = default;
		/// @brief Creates a quaternion with the given dimensions.
		/// @param x The X dimension of the quaternion.
		/// @param y The Y dimension of the quaternion.
		/// @param z The Z dimension of the quaternion.
		/// @param w The W dimension of the quaternion.
		Quaternion(float x, float y, float z, float w);
		/// @brief Creates a quaternion with the given elements.
		/// @param elements The elements of the quaternion.
		Quaternion(const float elements[4]);

		/// @brief Converts the given Vector4 into a quaternion.
		/// @param other The Vector4 to convert.
		explicit Quaternion(const Vector4& other);

		/// @brief Copies the given quaternion's contents into this quaternion.
		/// @param other The quaternion to copy.
		/// @return A reference to this quaternion.
		Quaternion& operator=(const Quaternion& other) = default;
		/// @brief Moves the given quaternion's contents into this quaternion.
		/// @param other The quaternion to move.
		/// @return A reference to this quaternion.
		Quaternion& operator=(Quaternion&& other) noexcept = default;

		/// @brief Compares the two quaternions.
		/// @param other The quaternion to compare with.
		/// @return True if the two quaternions are equal, otherwise false.
		bool operator==(const Quaternion& other) const;
		/// @brief Compares the two quaternions.
		/// @param other The quaternion to compare with.
		/// @return True if the two quaternions are different, otherwise false.
		bool operator!=(const Quaternion& other) const;

		/// @brief Multiplies the two given quaternions.
		/// @param other The quaternion to multiply with.
		/// @return The resulting quaternion.
		Quaternion operator*(const Quaternion& other) const;
		/// @brief Multiplies the given quaternion with this quaternion.
		/// @param other The quaternion to multiply with.
		/// @return A reference to this quaternion.
		Quaternion& operator*=(const Quaternion& other);

		/// @brief Calculates the quaternion's magnitude.
		/// @return The quaternion's magnitude.
		float Magnitude() const;
		/// @brief Calculates the square of the quaternion's magnitude.
		/// @return The square of the quaternion's magnitude.
		float SqrMagnitude() const;
		/// @brief Calculates the inverse of the quaternion's magnitude.
		/// @return The square of the quaternion's magnitude.
		float InvMagnitude() const;

		/// @brief Reduces the quaternion's magnitude to 1 while maintaining its proportions.
		/// @return A reference to this quaternion.
		Quaternion& Normalize();
		/// @brief Calculates a quaternion with a magnitude of 1 and the same proportions as this quaternion.
		/// @return The resulting quaternion.
		Quaternion Normalized() const;
		/// @brief Inverts this quaternion.
		/// @return A reference to this quaternion.
		Quaternion& Invert();
		/// @brief Creates a new quaternion equal to the inverted version of this quaternion.
		/// @return The resulting quaternion.
		Quaternion Inverted() const;

		/// @brief Calculates the dot product of the two quaternions.
		/// @param other The other quaternion.
		/// @return The dot product of the two quaternions.
		float Dot(const Quaternion& other) const;

		/// @brief Destroys this quaternion.
		~Quaternion() = default;

		/// @brief Generates a quaternion that represents the rotation around an axis.
		/// @param axis The axis to rotate around. Must be normalized.
		/// @param angle The angle of rotation, in radians.
		/// @return The resulting quaternion.
		static Quaternion AroundAxis(Vector3 axis, float angle);
		/// @brief Generates a quaternion that represents the rotation of the given euler angles.
		/// @param eulerAngles The euler angles, in radians.
		/// @return The resulting quaternion.
		static Quaternion EulerAngles(Vector3 eulerAngles);
	};
}