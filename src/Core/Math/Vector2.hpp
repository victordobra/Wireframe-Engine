#pragma once

#include "Core/Types/Defines.hpp"

namespace wfe {
	class Vector3;
	class Vector4;

	/// @brief A class that holds a 2D vector
	class Vector2 {
	public:
		/// @brief A Vector2 with all elements set to 0.
		static const Vector2 ZERO;
		/// @brief A Vector2 with all elements set to 1.
		static const Vector2 ONE;
		/// @brief A Vector2 pointing in the up direction.
		static const Vector2 UP;
		/// @brief A Vector2 pointing in the down direction.
		static const Vector2 DOWN;
		/// @brief A Vector2 pointing in the left direction.
		static const Vector2 LEFT;
		/// @brief A Vector2 pointing in the right direction.
		static const Vector2 RIGHT;

		union {
			/// @brief An array containing all of the vector's elements.
			float elements[2];

			struct {
				/// @brief The X dimension of the vector.
				float x;
				/// @brief The Y dimension of the vector.
				float y;
			};
		};

		/// @brief Constructs a Vector2 with all values set to 0.
		Vector2();
		/// @brief Copies the given Vector2.
		/// @param other The Vector2 to copy.
		Vector2(const Vector2& other) = default;
		/// @brief Moves the given Vector2's contents.
		/// @param other The Vector2 to move.
		Vector2(Vector2&& other) noexcept = default;
		/// @brief Creates a Vector2 with all dimensions set to the given value.
		/// @param x The value to set every dimension to.
		Vector2(float x);
		/// @brief Creates a Vector2 with the given dimensions.
		/// @param x The X dimension of the Vector2.
		/// @param y The Y dimension of the Vector2.
		Vector2(float x, float y);
		/// @brief Creates a Vector2 with the given dimensions.
		/// @param elements An array with the Vector2's elements
		Vector2(const float elements[2]);

		/// @brief Converts the given Vector3 into a Vector2.
		/// @param other The Vector3 to convert.
		explicit Vector2(const Vector3& other);
		/// @brief Converts the given Vector4 into a Vector2.
		/// @param other The Vector4 to convert.
		explicit Vector2(const Vector4& other);

		/// @brief Copies the given Vector2's elements into this Vector2.
		/// @param other The Vector2 to copy from.
		/// @return A reference to this Vector2.
		Vector2& operator=(const Vector2& other) = default;
		/// @brief Moves the given Vector2's contents into this Vector2.
		/// @param other The Vector2 to move from.
		/// @return A reference to this Vector2.
		Vector2& operator=(Vector2&& other) noexcept = default;

		/// @brief Compares the two Vector2s.
		/// @param other The Vector2 to compare with.
		/// @return True if the two Vector2s are equal, otherwise false.
		bool operator==(const Vector2& other) const;
		/// @brief Compares the two Vector2s.
		/// @param other The Vector2 to compare with.
		/// @return True if the two Vector2s are different, otherwise false.
		bool operator!=(const Vector2& other) const;

		/// @brief Adds the two Vector2s together.
		/// @param other The Vector2 to add.
		/// @return The resulting Vector2, with each of its dimensions being the sum of both Vector2s' corresponding dimensions.
		Vector2 operator+(const Vector2& other) const;
		/// @brief Subtracts the two Vector2s.
		/// @param other The Vector2 to subtract.
		/// @return The resulting Vector2, with each of its dimensions being the difference of both Vector2s' corresponding dimensions.
		Vector2 operator-(const Vector2& other) const;
		/// @brief Calculates the Vector2's opposite dimensions.
		/// @return The resulting Vector2, with each of its dimensions being the opposite of the corresponding dimension of this Vector2.
		Vector2 operator-() const;
		/// @brief Multiplies the two Vector2s together.
		/// @param other The Vector2 to multiply.
		/// @return The resulting Vector2, with each of its dimensions being the product of both Vector2s' corresponding dimensions.
		Vector2 operator*(const Vector2& other) const;
		/// @brief Divides the two Vector2s.
		/// @param other The Vector2 to divide.
		/// @return The resulting Vector2, with each of its dimensions being the dividend of both Vector2s' corresponding dimensions.
		Vector2 operator/(const Vector2& other) const;

		/// @brief Sums each of this Vector2's dimensions with the given Vector2's corresponding dimensions.
		/// @param other The Vector2 to add to this Vector2.
		/// @return A reference to this Vector2.
		Vector2& operator+=(const Vector2& other);
		/// @brief Subtracts from each of this Vector2's dimensions the given Vector2's corresponding dimensions.
		/// @param other The Vector2 to subtract from this Vector2.
		/// @return A reference to this Vector2.
		Vector2& operator-=(const Vector2& other);
		/// @brief Multiplies each of this Vector2's dimensions with the given Vector2's corresponding dimensions.
		/// @param other The Vector2 to multiply to this Vector2.
		/// @return A reference to this Vector2.
		Vector2& operator*=(const Vector2& other);
		/// @brief Multiplies each of this Vector2's dimensions with the given value.
		/// @param other The value to multiply to this Vector2.
		/// @return A reference to this Vector2.
		Vector2& operator*=(float other);
		/// @brief Divides from each of this Vector2's dimensions the given Vector2's corresponding dimensions.
		/// @param other The Vector2 to divide from this Vector2.
		Vector2& operator/=(const Vector2& other);
		/// @brief Divides from each of this Vector2's dimensions the given value.
		/// @param other The value to divide from this Vector2.
		/// @return A reference to this Vector2.
		Vector2& operator/=(float other);
		
		/// @brief Calculates the magnitude of this Vector2.
		/// @return The magnitude of this Vector2.
		float Magnitude() const;
		/// @brief Calculates the square of this Vector2's magnitude.
		/// @return The square of this Vector2's magnitude.
		float SqrMagnitude() const;
		/// @brief Calculates the inverse of this Vector2's magnitude.
		/// @return The inverse of this Vector2's magnitude.
		float InvMagnitude() const;

		/// @brief Reduces this Vector2's magnitude to 1 while maintaining its proportions.
		/// @return A reference to this Vector2.
		Vector2& Normalize();
		/// @brief Calculates a Vector2 with a magnitude to 1 and the same proportions as this vector.
		/// @return The resulting Vector2.
		Vector2 Normalized() const;
		/// @brief Flips this Vector2 by setting each coordinate to its opposite.
		/// @return A reference to this Vector2.
		Vector2& Flip();
		/// @brief Calculates a Vector2 whose coordinates are the opposites the this Vector2's coordinates.
		/// @return The resulting Vector2.
		Vector2 Flipped() const;

		/// @brief Calculates the dot product of the two Vector2s.
		/// @param other The other Vector2.
		/// @return The dot product of the two Vector2s.
		float Dot(const Vector2& other) const;
		/// @brief Calculates the angle between the two Vector2s.
		/// @param other The other Vector2.
		/// @return The angle, in radians, between the two Vector2s.
		float Angle(const Vector2& other) const;

		/// @brief Destroys the Vector2.
		~Vector2() = default;
	};

	/// @brief Multiplies the given Vector2 and the given value.
	/// @param vec The vector to multiply.
	/// @param val The value to multiply with.
	/// @return The resulting Vector2, with each of its dimensions being the product between the given Vector2's corresponding dimension and the given value.
	Vector2 operator*(const Vector2& vec, float val);
	/// @brief Multiplies the given Vector2 and the given value.
	/// @param val The value to multiply with.
	/// @param vec The vector to multiply.
	/// @return The resulting Vector2, with each of its dimensions being the product between the given Vector2's corresponding dimension and the given value.
	Vector2 operator*(float val, const Vector2& vec);
	/// @brief Divides the given value from the given Vector2.
	/// @param vec The vector to divide from.
	/// @param val The value to divide with.
	/// @return The resulting Vector2, with each of its dimensions being the dividen of the given Vector2's corresponding dimension and the given value.
	Vector2 operator/(const Vector2& vec, float val);
	/// @brief Divides the given value from the given Vector2.
	/// @param vec The vector to divide from.
	/// @param val The value to divide with.
	/// @return The resulting Vector2, with each of its dimensions being the dividen of the given value and the given Vector2's corresponding dimension.
	Vector2 operator/(float val, const Vector2& vec);
}

template<>
struct std::hash<wfe::Vector2> {
	std::size_t operator()(const wfe::Vector2& vec) {
		// Use the boost library hash combine to mix all hashes
		std::hash<float> hasher;
		size_t res = 0;
		for(size_t i = 0; i != 2; ++i)
			res ^= hasher(vec.elements[i]) + 0x9e3779b9 + (res << 6) + (res >> 2);
		
		return res;
	}
};