#pragma once

#include "Core/Types/Defines.hpp"

namespace wfe {
	class Vector2;
	class Vector3;
	class Quaternion;

	/// @brief A class that holds a 2D vector
	class Vector4 {
	public:
		/// @brief A Vector4 with all elements set to 0.
		static const Vector4 ZERO;
		/// @brief A Vector4 with all elements set to 1.
		static const Vector4 ONE;

		union {
			/// @brief An array containing all of the vector's elements.
			float elements[4];

			struct {
				/// @brief The X dimension of the vector.
				float x;
				/// @brief The Y dimension of the vector.
				float y;
				/// @brief The Z dimension of the vector.
				float z;
				/// @brief The W dimension of the vector.
				float w;
			};
		};

		/// @brief Constructs a Vector4 with all values set to 0.
		Vector4();
		/// @brief Copies the given Vector4.
		/// @param other The Vector4 to copy.
		Vector4(const Vector4& other) = default;
		/// @brief Moves the given Vector4's contents.
		/// @param other The Vector4 to move.
		Vector4(Vector4&& other) noexcept = default;
		/// @brief Creates a Vector4 with all dimensions set to the given value.
		/// @param x The value to set every dimension to.
		Vector4(float x);
		/// @brief Creates a Vector4 with the given dimensions.
		/// @param x The X dimension of the Vector4.
		/// @param y The Y dimension of the Vector4.
		/// @param z The Z dimension of the Vector4.
		/// @param w The W dimension of the Vector4.
		Vector4(float x, float y, float z, float w);
		/// @brief Creates a Vector4 with the given dimensions.
		/// @param elements An array with the Vector4's elements
		Vector4(const float elements[4]);

		/// @brief Converts the given Vector2 into a Vector4.
		/// @param other The Vector2 to convert.
		Vector4(const Vector2& other);
		/// @brief Converts the given Vector3 into a Vector4.
		/// @param other The Vector3 to convert.
		Vector4(const Vector3& other);
		/// @brief Converts the given quaternion into a Vector4.
		/// @param other The quaternion to convert.
		explicit Vector4(const Quaternion& other);

		/// @brief Copies the given Vector4's elements into this Vector4.
		/// @param other The Vector4 to copy from.
		/// @return A reference to this Vector4.
		Vector4& operator=(const Vector4& other) = default;
		/// @brief Moves the given Vector4's contents into this Vector4.
		/// @param other The Vector4 to move from.
		/// @return A reference to this Vector4.
		Vector4& operator=(Vector4&& other) noexcept = default;

		/// @brief Compares the two Vector4s.
		/// @param other The Vector4 to compare with.
		/// @return True if the two Vector4s are equal, otherwise false.
		bool operator==(const Vector4& other) const;
		/// @brief Compares the two Vector4s.
		/// @param other The Vector4 to compare with.
		/// @return True if the two Vector4s are different, otherwise false.
		bool operator!=(const Vector4& other) const;

		/// @brief Adds the two Vector4s together.
		/// @param other The Vector4 to add.
		/// @return The resulting Vector4, with each of its dimensions being the sum of both Vector4s' corresponding dimensions.
		Vector4 operator+(const Vector4& other) const;
		/// @brief Subtracts the two Vector4s.
		/// @param other The Vector4 to subtract.
		/// @return The resulting Vector4, with each of its dimensions being the difference of both Vector4s' corresponding dimensions.
		Vector4 operator-(const Vector4& other) const;
		/// @brief Calculates the Vector4's opposite dimensions.
		/// @return The resulting Vector4, with each of its dimensions being the opposite of the corresponding dimension of this Vector4.
		Vector4 operator-() const;
		/// @brief Multiplies the two Vector4s together.
		/// @param other The Vector4 to multiply.
		/// @return The resulting Vector4, with each of its dimensions being the product of both Vector4s' corresponding dimensions.
		Vector4 operator*(const Vector4& other) const;
		/// @brief Divides the two Vector4s.
		/// @param other The Vector4 to divide.
		/// @return The resulting Vector4, with each of its dimensions being the dividend of both Vector4s' corresponding dimensions.
		Vector4 operator/(const Vector4& other) const;

		/// @brief Sums each of this Vector4's dimensions with the given Vector4's corresponding dimensions.
		/// @param other The Vector4 to add to this Vector4.
		/// @return A reference to this Vector4.
		Vector4& operator+=(const Vector4& other);
		/// @brief Subtracts from each of this Vector4's dimensions the given Vector4's corresponding dimensions.
		/// @param other The Vector4 to subtract from this Vector4.
		/// @return A reference to this Vector4.
		Vector4& operator-=(const Vector4& other);
		/// @brief Multiplies each of this Vector4's dimensions with the given Vector4's corresponding dimensions.
		/// @param other The Vector4 to multiply to this Vector4.
		/// @return A reference to this Vector4.
		Vector4& operator*=(const Vector4& other);
		/// @brief Multiplies each of this Vector4's dimensions with the given value.
		/// @param other The value to multiply to this Vector4.
		/// @return A reference to this Vector4.
		Vector4& operator*=(float other);
		/// @brief Divides from each of this Vector4's dimensions the given Vector4's corresponding dimensions.
		/// @param other The Vector4 to divide from this Vector4.
		Vector4& operator/=(const Vector4& other);
		/// @brief Divides from each of this Vector4's dimensions the given value.
		/// @param other The value to divide from this Vector4.
		/// @return A reference to this Vector4.
		Vector4& operator/=(float other);
		
		/// @brief Calculates the magnitude of this Vector4.
		/// @return The magnitude of this Vector4.
		float Magnitude() const;
		/// @brief Calculates the square of this Vector4's magnitude.
		/// @return The square of this Vector4's magnitude.
		float SqrMagnitude() const;
		/// @brief Calculates the inverse of this Vector4's magnitude.
		/// @return The inverse of this Vector4's magnitude.
		float InvMagnitude() const;

		/// @brief Reduces this Vector4's magnitude to 1 while maintaining its proportions.
		/// @return A reference to this Vector4.
		Vector4& Normalize();
		/// @brief Calculates a Vector4 with a magnitude to 1 and the same proportions as this vector.
		/// @return The resulting Vector4.
		Vector4 Normalized() const;
		/// @brief Flips this Vector4 by setting each coordinate to its opposite.
		/// @return A reference to this Vector4.
		Vector4& Flip();
		/// @brief Calculates a Vector4 whose coordinates are the opposites the this Vector4's coordinates.
		/// @return The resulting Vector4.
		Vector4 Flipped() const;

		/// @brief Calculates the dot product of the two Vector4s.
		/// @param other The other Vector4.
		/// @return The dot product of the two Vector4s.
		float Dot(const Vector4& other) const;
		/// @brief Calculates the angle between the two Vector4s.
		/// @param other The other Vector4.
		/// @return The angle between the two Vector4s.
		float Angle(const Vector4& other) const;

		/// @brief Destroys the Vector4.
		~Vector4() = default;
	};

	/// @brief Multiplies the given Vector4 and the given value.
	/// @param vec The vector to multiply.
	/// @param val The value to multiply with.
	/// @return The resulting Vector4, with each of its dimensions being the product between the given Vector4's corresponding dimension and the given value.
	Vector4 operator*(const Vector4& vec, float val);
	/// @brief Multiplies the given Vector4 and the given value.
	/// @param val The value to multiply with.
	/// @param vec The vector to multiply.
	/// @return The resulting Vector4, with each of its dimensions being the product between the given Vector4's corresponding dimension and the given value.
	Vector4 operator*(float val, const Vector4& vec);
	/// @brief Divides the given value from the given Vector4.
	/// @param vec The vector to divide from.
	/// @param val The value to divide with.
	/// @return The resulting Vector4, with each of its dimensions being the dividen of the given Vector4's corresponding dimension and the given value.
	Vector4 operator/(const Vector4& vec, float val);
	/// @brief Divides the given value from the given Vector4.
	/// @param vec The vector to divide from.
	/// @param val The value to divide with.
	/// @return The resulting Vector4, with each of its dimensions being the dividen of the given value and the given Vector4's corresponding dimension.
	Vector4 operator/(float val, const Vector4& vec);
}