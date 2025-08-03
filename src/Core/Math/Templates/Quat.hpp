#pragma once

#include "Core/Types/Defines.hpp"

namespace wfe {
	/// @brief A class template for a mathematical quaternion, using the numerical type T.
	/// @tparam T The numerical type of the quaternion's components.
	template<class T>
	class Quat {
	public:
		/// @brief The X component of the quaternion (the multiple of the i basis vector).
		T x{};
		/// @brief The Y component of the quaternion (the multiple of the j basis vector).
		T y{};
		/// @brief The Z component of the quaternion (the multiple of the k basis vector).
		T z{};
		/// @brief The W component of the quaternion (the multiple of the 1 basis vector).
		T w{};

		/// @brief Gets the quaternion's component with the given index.
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
				throw std::out_of_range("Quaternion index is out of range!");
			}
		}
		/// @brief Gets the quaternion's component with the given index.
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
				throw std::out_of_range("Quaternion index is out of range!");
			}
		}

		/// @brief Gets the positive quaternion.
		/// @return The positive quaternion.
		constexpr Quat operator+() const {
			return { x, y, z, w };
		}
		/// @brief Gets the negative quaternion.
		/// @return The negative quaternion.
		constexpr Quat operator-() const {
			return { -x, -y, -z, -w };
		}

		/// @brief Adds the given quaternion to this quaternion.
		/// @param other The quaternion to add.
		/// @return A reference to this quaternion.
		constexpr Quat& operator+=(const Quat& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;

			return *this;
		}
		/// @brief Decreases the given quaternion from this quaternion.
		/// @param other The quaternion to subtract.
		/// @return A reference to this quaternion.
		constexpr Quat& operator-=(const Quat& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;

			return *this;
		}
		/// @brief Multiplies this quaternion by the given scalar.
		/// @param other The scalar to multiply by.
		/// @return A reference to this quaternion.
		constexpr Quat& operator*=(const T& other) {
			x *= other;
			y *= other;
			z *= other;
			w *= other;

			return *this;
		}
		/// @brief Multiplies this quaternion by the given quaternion.
		/// @param other The quaternion to multiply by.
		/// @return A reference to this quaternion.
		constexpr Quat& operator*=(const Quat& other) {
			T prevX = x, prevY = y, prevZ = z, prevW = w;

			x =  prevX * other.w + prevY * other.z - prevZ * other.y + prevW * other.x;
			y = -prevX * other.z + prevY * other.w + prevZ * other.x + prevW * other.y;
			z =  prevX * other.y - prevY * other.x + prevZ * other.w + prevW * other.z;
			w = -prevX * other.z - prevY * other.y - prevZ * other.z + prevW * other.w;

			return *this;
		}
		/// @brief Divides this quaternion by the given scalar.
		/// @param other The scalar to divide by.
		/// @return A reference to this quaternion.
		constexpr Quat& operator/=(const T& other) {
			x /= other;
			y /= other;
			z /= other;
			w /= other;

			return *this;
		}
	};

	/// @brief Checks if two quaternions are equal.
	/// @tparam T The numerical type of the quaternion's components.
	/// @param quat1 The first quaternion to compare.
	/// @param quat2 The second quaternion to compare.
	/// @return True if the quaternions are equal, otherwise false.
	template<class T>
	inline constexpr bool operator==(const Quat<T>& quat1, const Quat<T>& quat2) {
		return quat1.x == quat2.x && quat1.y == quat2.y && quat1.z == quat2.z && quat1.w == quat2.w;
	}
	/// @brief Checks if two quaternions are different.
	/// @tparam T The numerical type of the quaternion's components.
	/// @param quat1 The first quaternion to compare.
	/// @param quat2 The second quaternion to compare.
	/// @return True if the quaternions are different, otherwise false.
	template<class T>
	inline constexpr bool operator!=(const Quat<T>& quat1, const Quat<T>& quat2) {
		return quat1.x != quat2.x || quat1.y != quat2.y || quat1.z != quat2.z || quat1.w != quat2.w;
	}

	/// @brief Adds two quaternions.
	/// @tparam T The numerical type of the quaternion's components.
	/// @param quat1 The first quaternion to add.
	/// @param quat2 The second quaternion to add.
	/// @return The resulting quaternion.
	template<class T>
	inline constexpr Quat<T> operator+(const Quat<T>& quat1, const Quat<T>& quat2) {
		return { quat1.x + quat2.x, quat1.y + quat2.y, quat1.z + quat2.z, quat1.w + quat2.w };
	}
	/// @brief Subtracts two quaternions.
	/// @tparam T The numerical type of the quaternion's components.
	/// @param quat1 The quaternion to subtract from.
	/// @param quat2 The quaternion tu subtract.
	/// @return The resulting quaternion.
	template<class T>
	inline constexpr Quat<T> operator-(const Quat<T>& quat1, const Quat<T>& quat2) {
		return { quat1.x - quat2.x, quat1.y - quat2.y, quat1.z - quat2.z, quat1.w - quat2.w };
	}
	/// @brief Multiplies a quaternion by a scalar.
	/// @tparam T The numerical type of the quaternion's components.
	/// @param quat The quaternion to multiply.
	/// @param scalar The scalar to multiply the quaternion by.
	/// @return The resulting quaternion.
	template<class T>
	inline constexpr Quat<T> operator*(const Quat<T>& quat, const T& scalar) {
		return { quat.x * scalar, quat.y * scalar, quat.z * scalar, quat.w * scalar };
	}
	/// @brief Multiplies a quaternion by a scalar.
	/// @tparam T The numerical type of the quaternion's components.
	/// @param scalar The scalar to multiply the quaternion by.
	/// @param quat The quaternion to multiply.
	/// @return The resulting quaternion.
	template<class T>
	inline constexpr Quat<T> operator*(const T& scalar, const Quat<T>& quat) {
		return { scalar * quat.x, scalar * quat.y, scalar * quat.z, scalar * quat.w };
	}
	/// @brief Multiplies two quaternions.
	/// @tparam T The numerical type of the quaternion's components.
	/// @param quat1 The first quaternion to multiply.
	/// @param quat2 The second quaternion to multiply.
	/// @return The resulting quaternion.
	template<class T>
	inline constexpr Quat<T> operator*(const Quat<T>& quat1, const Quat<T>& quat2) {
		return {
			 quat1.x * quat2.w + quat1.y * quat2.z - quat1.z * quat2.y + quat1.w * quat2.x,
			-quat1.x * quat2.z + quat1.y * quat2.w + quat1.z * quat2.x + quat1.w * quat2.y,
			 quat1.x * quat2.y - quat1.y * quat2.x + quat1.z * quat2.w + quat1.w * quat2.z,
			-quat1.x * quat2.z - quat1.y * quat2.y - quat1.z * quat2.z + quat1.w * quat2.w
		};
	}
	/// @brief Divides a quaternion by a scalar.
	/// @tparam T The numerical type of the quaternion's components.
	/// @param quat The quaternion to divide.
	/// @param scalar The scalar to divide the quaternion by.
	/// @return The resulting quaternion.
	template<class T>
	inline constexpr Quat<T> operator/(const Quat<T>& quat, const T& scalar) {
		return { quat.x / scalar, quat.y / scalar, quat.z / scalar, quat.w / scalar };
	}

	/// @brief The quaternion type using single precision floating point values.
	typedef Quat<float> Quatf;
	/// @brief The quaternion type using double precision floating point values.
	typedef Quat<double> Quatd;
	/// @brief The quaternion type using unsigned integer values.
	typedef Quat<uint32_t> Quatu;
	/// @brief The quaternion type using signed integer values.
	typedef Quat<int32_t> Quati;
}

template<class T>
struct std::hash<wfe::Quat<T>> {
	std::size_t operator()(const wfe::Quat<T>& quat) const {
		// Combine the hashes of all of the quaternion's components
		std::hash<T> hasher;

		std::size_t res = hasher(quat.x);
		res ^= hasher(quat.y) + 0x9e3779b9 + (res << 6) + (res >> 2);
		res ^= hasher(quat.z) + 0x9e3779b9 + (res << 6) + (res >> 2);
		res ^= hasher(quat.w) + 0x9e3779b9 + (res << 6) + (res >> 2);

		return res;
	}
};