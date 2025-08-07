#pragma once

#include "Vec.hpp"
#include "Core/Types/Defines.hpp"
#include <stdexcept>

namespace wfe {
	/// @brief A general template for a four-dimensional vector, using the numerical type T.
	/// @tparam T The numerical type of the vector's components.
	template<class T>
	class Vec<4, T> {
	public:
		/// @brief The X component of the vector.
		T x;
		/// @brief The Y component of the vector.
		T y;
		/// @brief The Z component of the vector.
		T z;
		/// @brief The W component of the vector.
		T w;

		/// @brief Gets the vector's component with the given index.
		/// @param index The index of the component to find.
		/// @return A reference to the requested component.
		constexpr T& operator[](size_t index) {
			switch(index) {
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			default:
				throw std::out_of_range("Vector index is out of range!");
			}
		}
		/// @brief Gets the vector's component with the given index.
		/// @param index The index of the component to find.
		/// @return A const reference to the requested component.
		constexpr const T& operator[](size_t index) const {
			switch(index) {
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			default:
				throw std::out_of_range("Vector index is out of range!");
			}
		}

		/// @brief Gets the positive vector.
		/// @return The positive vector.
		constexpr Vec operator+() const {
			return { x, y, z, w };
		}
		/// @brief Gets the negative vector.
		/// @return The negative vector.
		constexpr Vec operator-() const {
			return { -x, -y, -z, -w };
		}

		/// @brief Adds the given value to each component of the vector.
		/// @param other The value to add to each component.
		/// @return A reference to this vector.
		constexpr Vec& operator+=(const T& other) {
			x += other;
			y += other;
			z += other;
			w += other;

			return *this;
		}
		/// @brief Adds the components of the given vector to this vector's corresponding components.
		/// @param other The vector to add.
		/// @return A reference to this vector.
		constexpr Vec& operator+=(const Vec& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;

			return *this;
		}
		/// @brief Subtracts the given value from each component of the vector.
		/// @param other The value to subtract from each component.
		/// @return A reference to this vector.
		constexpr Vec& operator-=(const T& other) {
			x -= other;
			y -= other;
			z -= other;
			w -= other;

			return *this;
		}
		/// @brief Subtracts the components of the given vector from this vector's corresponding components.
		/// @param other The vector to subtract.
		/// @return A reference to this vector.
		constexpr Vec& operator-=(const Vec& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;

			return *this;
		}
		/// @brief Multiplies each component of the vector by the given value.
		/// @param other The value to multiply to each component by.
		/// @return A reference to this vector.
		constexpr Vec& operator*=(const T& other) {
			x *= other;
			y *= other;
			z *= other;
			w *= other;

			return *this;
		}
		/// @brief Multiplies the components of this vector given vector by the given vector's corresponding components.
		/// @param other The vector to multiply by.
		/// @return A reference to this vector.
		constexpr Vec& operator*=(const Vec& other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;

			return *this;
		}
		/// @brief Divides each component of the vector by the given value.
		/// @param other The value to divide each component by.
		/// @return A reference to this vector.
		constexpr Vec& operator/=(const T& other) {
			x /= other;
			y /= other;
			z /= other;
			w /= other;

			return *this;
		}
		/// @brief Divides the components of this vector given vector by the given vector's corresponding components.
		/// @param other The vector to divide by.
		/// @return A reference to this vector.
		constexpr Vec& operator/=(const Vec& other) {
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;

			return *this;
		}

		/// @brief Converts this vector to a two-dimensional vector, ignoring the Z and W components.
		/// @return The two-dimensional vector resulting from the conversion.
		constexpr explicit operator Vec<2, T>() const {
			return { x, y };
		}
		/// @brief Converts this vector to a three-dimensional vector, ignoring the W component.
		/// @return The three-dimensional vector resulting from the conversion.
		constexpr explicit operator Vec<3, T>() const {
			return { x, y, z };
		}
	};

	/// @brief Checks if two vectors are equal.
	/// @tparam T The numerical type of the vector's components.
	/// @param vec1 The first vector to compare.
	/// @param vec2 The second vector to compare.
	/// @return True if the vectors are equal, otherwise false.
	template<class T>
	inline constexpr bool operator==(const Vec<4, T>& vec1, const Vec<4, T>& vec2) {
		return vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z && vec1.w == vec2.w;
	}
	/// @brief Checks if two vectors are different.
	/// @tparam T The numerical type of the vector's components.
	/// @param vec1 The first vector to compare.
	/// @param vec2 The second vector to compare.
	/// @return True if the vectors are different, otherwise false.
	template<class T>
	inline constexpr bool operator!=(const Vec<4, T>& vec1, const Vec<4, T>& vec2) {
		return vec1.x != vec2.x || vec1.y != vec2.y || vec1.z != vec2.z || vec1.w != vec2.w;
	}

	/// @brief Adds a scalar value to the components of a vector.
	/// @tparam T The numerical type of the vector's components.
	/// @param vec The vector to add the scalar to.
	/// @param scalar The scalar value to add to each component of the vector.
	/// @return The resulting vector.
	template<class T>
	inline constexpr Vec<4, T> operator+(const Vec<4, T>& vec, const T& scalar) {
		return { vec.x + scalar, vec.y + scalar, vec.z + scalar, vec.w + scalar };
	}
	/// @brief Adds a scalar value to the components of a vector.
	/// @tparam T The numerical type of the vector's components.
	/// @param scalar The scalar value to add to each component of the vector.
	/// @param vec The vector to add the scalar to.
	/// @return The resulting vector.
	template<class T>
	inline constexpr Vec<4, T> operator+(const T& scalar, const Vec<4, T>& vec) {
		return { scalar + vec.x, scalar + vec.y, scalar + vec.z, vec.w + scalar };
	}
	/// @brief Adds two vectors.
	/// @tparam T The numerical type of the vector's components.
	/// @param vec1 The first vector to add.
	/// @param vec2 The second vector to add.
	/// @return The resulting vector.
	template<class T>
	inline constexpr Vec<4, T> operator+(const Vec<4, T>& vec1, const Vec<4, T>& vec2) {
		return { vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z, vec1.w + vec2.w };
	}
	/// @brief Subtracts a scalar value from the components of a vector.
	/// @tparam T The numerical type of the vector's components.
	/// @param vec The vector to subtract the scalar from.
	/// @param scalar The scalar value to subtract from each component of the vector.
	/// @return The resulting vector.
	template<class T>
	inline constexpr Vec<4, T> operator-(const Vec<4, T>& vec, const T& scalar) {
		return { vec.x - scalar, vec.y - scalar, vec.z - scalar, vec.w - scalar };
	}
	/// @brief Subtracts each of the components of a vector from a scalar value.
	/// @tparam T The numerical type of the vector's components.
	/// @param scalar The scalar value to subtract the vector's components from.
	/// @param vec The vector to subtract.
	/// @return The resulting vector.
	template<class T>
	inline constexpr Vec<4, T> operator-(const T& scalar, const Vec<4, T>& vec) {
		return { scalar - vec.x, scalar - vec.y, scalar - vec.z, scalar - vec.w };
	}
	/// @brief Subtracts two vectors.
	/// @tparam T The numerical type of the vector's components.
	/// @param vec1 The vector to subtract from.
	/// @param vec2 The vector to subtract.
	/// @return The resulting vector.
	template<class T>
	inline constexpr Vec<4, T> operator-(const Vec<4, T>& vec1, const Vec<4, T>& vec2) {
		return { vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z, vec1.w - vec2.w };
	}
	/// @brief Multiplies each of the components of a vector by a scalar value.
	/// @tparam T The numerical type of the vector's components.
	/// @param vec The vector to multiply.
	/// @param scalar The scalar value to multiply each component of the vector by.
	/// @return The resulting vector.
	template<class T>
	inline constexpr Vec<4, T> operator*(const Vec<4, T>& vec, const T& scalar) {
		return { vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar };
	}
	/// @brief Multiplies each of the components of a vector by a scalar value.
	/// @tparam T The numerical type of the vector's components.
	/// @param scalar The scalar value to multiply each component of the vector by.
	/// @param vec The vector to multiply.
	/// @return The resulting vector.
	template<class T>
	inline constexpr Vec<4, T> operator*(const T& scalar, const Vec<4, T>& vec) {
		return { scalar * vec.x, scalar * vec.y, scalar * vec.z, scalar * vec.w };
	}
	/// @brief Multiplies two vectors component-wise.
	/// @tparam T The numerical type of the vector's components.
	/// @param vec1 The first vector to multiply.
	/// @param vec2 The second vector to multiply.
	/// @return The resulting vector.
	template<class T>
	inline constexpr Vec<4, T> operator*(const Vec<4, T>& vec1, const Vec<4, T>& vec2) {
		return { vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z, vec1.w * vec2.w };
	}
	/// @brief Divides each of the components of a vector by a scalar value.
	/// @tparam T The numerical type of the vector's components.
	/// @param vec The vector to divide.
	/// @param scalar The scalar value to divide each component of the vector by.
	/// @return The resulting vector.
	template<class T>
	inline constexpr Vec<4, T> operator/(const Vec<4, T>& vec, const T& scalar) {
		return { vec.x / scalar, vec.y / scalar, vec.z / scalar, vec.w / scalar };
	}
	/// @brief Divides a scalar value by each of the components of a vector.
	/// @tparam T The numerical type of the vector's components.
	/// @param scalar The scalar value to divide.
	/// @param vec The vector to divide by.
	/// @return The resulting vector.
	template<class T>
	inline constexpr Vec<4, T> operator/(const T& scalar, const Vec<4, T>& vec) {
		return { scalar / vec.x, scalar / vec.y, scalar / vec.z, scalar / vec.w };
	}
	/// @brief Divides two vectors component-wise.
	/// @tparam T The numerical type of the vector's components.
	/// @param vec1 The vector to divide.
	/// @param vec2 The vector to divide by.
	/// @return The resulting vector.
	template<class T>
	inline constexpr Vec<4, T> operator/(const Vec<4, T>& vec1, const Vec<4, T>& vec2) {
		return { vec1.x / vec2.x, vec1.y / vec2.y, vec1.z / vec2.z, vec1.w / vec2.w };
	}

	/// @brief The four-dimensional vector type using single precision floation point values.
	typedef Vec<4, float> Vec4f;
	/// @brief The four-dimensional vector type using double precision floation point values.
	typedef Vec<4, double> Vec4d;
	/// @brief The four-dimensional vector type using unsigned integer values.
	typedef Vec<4, uint32_t> Vec4u;
	/// @brief The four-dimensional vector type using signed integer values.
	typedef Vec<4, int32_t> Vec4i;
}

template<class T>
struct std::hash<wfe::Vec<4, T>> {
	std::size_t operator()(const wfe::Vec<4, T>& vec) const {
		// Combine the hashes of all of the vector's components
		std::hash<T> hasher;

		std::size_t res = hasher(vec.x);
		res ^= hasher(vec.y) + 0x9e3779b9 + (res << 6) + (res >> 2);
		res ^= hasher(vec.z) + 0x9e3779b9 + (res << 6) + (res >> 2);
		res ^= hasher(vec.w) + 0x9e3779b9 + (res << 6) + (res >> 2);

		return res;
	}
};