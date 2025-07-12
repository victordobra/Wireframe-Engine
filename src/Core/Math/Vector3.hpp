#pragma once

#include "Core/Types/Defines.hpp"

namespace wfe {
	class Vector2;
	class Vector4;

	/// @brief A class that holds a 2D vector
	class Vector3 {
	public:
		/// @brief A Vector3 with all elements set to 0.
		static const Vector3 ZERO;
		/// @brief A Vector3 with all elements set to 1.
		static const Vector3 ONE;
		/// @brief A Vector3 pointing in the up direction.
		static const Vector3 UP;
		/// @brief A Vector3 pointing in the down direction.
		static const Vector3 DOWN;
		/// @brief A Vector3 pointing in the left direction.
		static const Vector3 LEFT;
		/// @brief A Vector3 pointing in the right direction.
		static const Vector3 RIGHT;
		/// @brief A Vector3 pointing in the forward direction.
		static const Vector3 FORWARD;
		/// @brief A Vector3 pointing in the back direction.
		static const Vector3 BACK;

		union {
			/// @brief An array containing all of the vector's elements.
			float elements[3];

			struct {
				/// @brief The X dimension of the vector.
				float x;
				/// @brief The Y dimension of the vector.
				float y;
				/// @brief The Z dimension of the vector.
				float z;
			};
		};

		/// @brief Constructs a Vector3 with all values set to 0.
		Vector3();
		/// @brief Copies the given Vector3.
		/// @param other The Vector3 to copy.
		Vector3(const Vector3& other) = default;
		/// @brief Moves the given Vector3's contents.
		/// @param other The Vector3 to move.
		Vector3(Vector3&& other) noexcept = default;
		/// @brief Creates a Vector3 with all dimensions set to the given value.
		/// @param x The value to set every dimension to.
		Vector3(float x);
		/// @brief Creates a Vector3 with the given dimensions.
		/// @param x The X dimension of the Vector3.
		/// @param y The Y dimension of the Vector3.
		Vector3(float x, float y);
		/// @brief Creates a Vector3 with the given dimensions.
		/// @param x The X dimension of the Vector3.
		/// @param y The Y dimension of the Vector3.
		/// @param z The Z dimension of the Vector3.
		Vector3(float x, float y, float z);
		/// @brief Creates a Vector3 with the given dimensions.
		/// @param elements An array with the Vector3's elements
		Vector3(const float elements[3]);

		/// @brief Converts the given Vector2 into a Vector3.
		/// @param other The Vector2 to convert.
		Vector3(const Vector2& other);
		/// @brief Converts the given Vector4 into a Vector3.
		/// @param other The Vector4 to convert.
		explicit Vector3(const Vector4& other);

		/// @brief Copies the given Vector3's elements into this Vector3.
		/// @param other The Vector3 to copy from.
		/// @return A reference to this Vector3.
		Vector3& operator=(const Vector3& other) = default;
		/// @brief Moves the given Vector3's contents into this Vector3.
		/// @param other The Vector3 to move from.
		/// @return A reference to this Vector3.
		Vector3& operator=(Vector3&& other) noexcept = default;

		/// @brief Compares the two Vector3s.
		/// @param other The Vector3 to compare with.
		/// @return True if the two Vector3s are equal, otherwise false.
		bool operator==(const Vector3& other) const;
		/// @brief Compares the two Vector3s.
		/// @param other The Vector3 to compare with.
		/// @return True if the two Vector3s are different, otherwise false.
		bool operator!=(const Vector3& other) const;

		/// @brief Adds the two Vector3s together.
		/// @param other The Vector3 to add.
		/// @return The resulting Vector3, with each of its dimensions being the sum of both Vector3s' corresponding dimensions.
		Vector3 operator+(const Vector3& other) const;
		/// @brief Subtracts the two Vector3s.
		/// @param other The Vector3 to subtract.
		/// @return The resulting Vector3, with each of its dimensions being the difference of both Vector3s' corresponding dimensions.
		Vector3 operator-(const Vector3& other) const;
		/// @brief Calculates the Vector3's opposite dimensions.
		/// @return The resulting Vector3, with each of its dimensions being the opposite of the corresponding dimension of this Vector3.
		Vector3 operator-() const;
		/// @brief Multiplies the two Vector3s together.
		/// @param other The Vector3 to multiply.
		/// @return The resulting Vector3, with each of its dimensions being the product of both Vector3s' corresponding dimensions.
		Vector3 operator*(const Vector3& other) const;
		/// @brief Divides the two Vector3s.
		/// @param other The Vector3 to divide.
		/// @return The resulting Vector3, with each of its dimensions being the dividend of both Vector3s' corresponding dimensions.
		Vector3 operator/(const Vector3& other) const;

		/// @brief Sums each of this Vector3's dimensions with the given Vector3's corresponding dimensions.
		/// @param other The Vector3 to add to this Vector3.
		/// @return A reference to this Vector3.
		Vector3& operator+=(const Vector3& other);
		/// @brief Subtracts from each of this Vector3's dimensions the given Vector3's corresponding dimensions.
		/// @param other The Vector3 to subtract from this Vector3.
		/// @return A reference to this Vector3.
		Vector3& operator-=(const Vector3& other);
		/// @brief Multiplies each of this Vector3's dimensions with the given Vector3's corresponding dimensions.
		/// @param other The Vector3 to multiply to this Vector3.
		/// @return A reference to this Vector3.
		Vector3& operator*=(const Vector3& other);
		/// @brief Multiplies each of this Vector3's dimensions with the given value.
		/// @param other The value to multiply to this Vector3.
		/// @return A reference to this Vector3.
		Vector3& operator*=(float other);
		/// @brief Divides from each of this Vector3's dimensions the given Vector3's corresponding dimensions.
		/// @param other The Vector3 to divide from this Vector3.
		Vector3& operator/=(const Vector3& other);
		/// @brief Divides from each of this Vector3's dimensions the given value.
		/// @param other The value to divide from this Vector3.
		/// @return A reference to this Vector3.
		Vector3& operator/=(float other);
		
		/// @brief Calculates the magnitude of this Vector3.
		/// @return The magnitude of this Vector3.
		float Magnitude() const;
		/// @brief Calculates the square of this Vector3's magnitude.
		/// @return The square of this Vector3's magnitude.
		float SqrMagnitude() const;
		/// @brief Calculates the inverse of this Vector3's magnitude.
		/// @return The inverse of this Vector3's magnitude.
		float InvMagnitude() const;

		/// @brief Reduces this Vector3's magnitude to 1 while maintaining its proportions.
		/// @return A reference to this Vector3.
		Vector3& Normalize();
		/// @brief Calculates a Vector3 with a magnitude to 1 and the same proportions as this vector.
		/// @return The resulting Vector3.
		Vector3 Normalized() const;
		/// @brief Flips this Vector3 by setting each coordinate to its opposite.
		/// @return A reference to this Vector3.
		Vector3& Flip();
		/// @brief Calculates a Vector3 whose coordinates are the opposites the this Vector3's coordinates.
		/// @return The resulting Vector3.
		Vector3 Flipped() const;

		/// @brief Calculates the dot product of the two Vector3s.
		/// @param other The other Vector3.
		/// @return The dot product of the two Vector3s.
		float Dot(const Vector3& other) const;
		/// @brief Calculates the angle between the two Vector3s.
		/// @param other The other Vector3.
		/// @return The angle between the two Vector3s.
		float Angle(const Vector3& other) const;
		/// @brief Calculates the cross product of the two Vector3s.
		/// @param other The other Vector3.
		/// @return The cross product of the two Vector3s.
		Vector3 Cross(const Vector3& other) const;

		/// @brief Destroys the Vector3.
		~Vector3() = default;
	};

	/// @brief Multiplies the given Vector3 and the given value.
	/// @param vec The vector to multiply.
	/// @param val The value to multiply with.
	/// @return The resulting Vector3, with each of its dimensions being the product between the given Vector3's corresponding dimension and the given value.
	Vector3 operator*(const Vector3& vec, float val);
	/// @brief Multiplies the given Vector3 and the given value.
	/// @param val The value to multiply with.
	/// @param vec The vector to multiply.
	/// @return The resulting Vector3, with each of its dimensions being the product between the given Vector3's corresponding dimension and the given value.
	Vector3 operator*(float val, const Vector3& vec);
	/// @brief Divides the given value from the given Vector3.
	/// @param vec The vector to divide from.
	/// @param val The value to divide with.
	/// @return The resulting Vector3, with each of its dimensions being the dividen of the given Vector3's corresponding dimension and the given value.
	Vector3 operator/(const Vector3& vec, float val);
	/// @brief Divides the given value from the given Vector3.
	/// @param vec The vector to divide from.
	/// @param val The value to divide with.
	/// @return The resulting Vector3, with each of its dimensions being the dividen of the given value and the given Vector3's corresponding dimension.
	Vector3 operator/(float val, const Vector3& vec);
}

template<>
struct std::hash<wfe::Vector3> {
	std::size_t operator()(const wfe::Vector3& vec) const {
		// Use the boost library hash combine to mix all hashes
		std::hash<float> hasher;
		size_t res = 0;
		for(size_t i = 0; i != 3; ++i)
			res ^= hasher(vec.elements[i]) + 0x9e3779b9 + (res << 6) + (res >> 2);
		
		return res;
	}
};