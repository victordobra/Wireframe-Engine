#pragma once
#include "Defines.h"

namespace mge {
	struct Vector3 {
		/// @brief Returns a vector with every element set as 0.
		static Vector3 Zero()      { return { 0.f,  0.f, 0.f }; }
		/// @brief Returns a vector with every element set as 1.
		static Vector3 One()       { return { 1.f,  1.f,  1.f }; }
		/// @brief Returns a vector that represents the up direction.
		static Vector3 Up()        { return { 0.f,  1.f,  0.f }; }
		/// @brief Returns a vector that represents the down direction.
		static Vector3 Down()      { return { 0.f, -1.f,  0.f }; }
		/// @brief Returns a vector that represents the left direction.
		static Vector3 Left()      { return { -1.f, 0.f,  0.f }; }
		/// @brief Returns a vector that represents the right direction.
		static Vector3 Right()     { return { 1.f,  0.f,  0.f }; }
		/// @brief Returns a vector that represents the forward direction.
		static Vector3 Forward()   { return { 0.f,  0.f, -1.f }; }
		/// @brief Returns a vector that represents the backwards direction.
		static Vector3 Backwards() { return { 0.f,  0.f,  1.f }; }

		union {
			float32_t elements[3]{ 0.f, 0.f, 0.f };

			struct {
				union {
					float32_t x, r, s, u;
				};
				union {
					float32_t y, g, t, v;
				};
				union {
					float32_t z, b, p, w;
				};
			};
		};

		Vector3() = default;
		Vector3(const Vector3&) = default;
		Vector3(Vector3&&) noexcept = default;
		Vector3(float32_t x) : x(x), y(x), z(x) { }
		Vector3(float32_t x, float32_t y, float32_t z = 0.f) : x(x), y(y), z(z) { }

		Vector3& operator=(const Vector3&) = default;
		Vector3& operator=(Vector3&&) noexcept = default;

		bool8_t operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z; }
		bool8_t operator==(Vector3&& other)      const { return x == other.x && y == other.y && z == other.z; }
		bool8_t operator!=(const Vector3& other) const { return x != other.x || y != other.y || z != other.z; }
		bool8_t operator!=(Vector3&& other)      const { return x != other.x || y != other.y || z != other.z; }

		Vector3 operator+(const Vector3& other)  const { return { x + other.x, y + other.y, z + other.z }; }
		Vector3 operator+(Vector3&& other)       const { return { x + other.x, y + other.y, z + other.z }; }
		Vector3 operator-(const Vector3& other)  const { return { x - other.x, y - other.y, z - other.z }; }
		Vector3 operator-(Vector3&& other)       const { return { x - other.x, y - other.y, z - other.z }; }
		Vector3 operator-()                      const { return { -x         , -y         , -z          }; }
		Vector3 operator*(const Vector3& other)  const { return { x * other.x, y * other.y, z * other.z }; }
		Vector3 operator*(Vector3&& other)       const { return { x * other.x, y * other.y, z * other.z }; }
		Vector3 operator*(float32_t other)       const { return { x * other  , y * other  , z * other   }; }
		Vector3 operator/(const Vector3& other)  const { return { x / other.x, y / other.y, z / other.z }; }
		Vector3 operator/(Vector3&& other)       const { return { x / other.x, y / other.y, z / other.z }; }
		Vector3 operator/(float32_t other)       const { return { x / other  , y / other  , z / other   }; }

		Vector3& operator+=(const Vector3& other)      { x += other.x; y += other.y; z += other.z; return *this; }
		Vector3& operator+=(Vector3&& other) noexcept  { x += other.x; y += other.y; z += other.z; return *this; }
		Vector3& operator-=(const Vector3& other)      { x -= other.x; y -= other.y; z -= other.z; return *this; }
		Vector3& operator-=(Vector3&& other) noexcept  { x -= other.x; y -= other.y; z -= other.z; return *this; }
		Vector3& operator*=(const Vector3& other)      { x *= other.x; y *= other.y; z *= other.z; return *this; }
		Vector3& operator*=(Vector3&& other) noexcept  { x *= other.x; y *= other.y; z *= other.z; return *this; }
		Vector3& operator*=(float32_t other)           { x *= other;   y *= other;   z *= other;   return *this; }
		Vector3& operator/=(const Vector3& other)      { x /= other.x; y /= other.y; z /= other.z; return *this; }
		Vector3& operator/=(Vector3&& other) noexcept  { x /= other.x; y /= other.y; z /= other.z; return *this; }
		Vector3& operator/=(float32_t other)           { x /= other;   y /= other;   z /= other;   return *this; }

		/// @brief Returns the length of the vector.
		float32_t Magnitude() const { return sqrtf(x * x + y * y + z * z); }
		/// @brief Returns the square of the length of the vector.
		float32_t SqrMagnitude() const { return x * x + y * y + z * z; }
		/// @brief Reduces the vector's length to 1.
		Vector3& Normalize() {
			float32_t sqrMag = SqrMagnitude();
			if (sqrMag == 1)
				return *this;

			uint32_t i;
			float32_t halfSqrMag = sqrMag * 0.5f, invMag = sqrMag;
			const float32_t threeHalves = 1.5f;

			memcpy(&i, &invMag, sizeof(uint32_t));
			i = 0x5f3759df - (i >> 1);
			memcpy(&invMag, &i, sizeof(uint32_t));
			invMag = invMag * (threeHalves - (halfSqrMag * invMag * invMag));
			invMag = invMag * (threeHalves - (halfSqrMag * invMag * invMag));

			x *= invMag;
			y *= invMag;
			z *= invMag;

			return *this;
		}
		/// @brief Returns a version of the vector with the length 1.
		Vector3 Normalized() const {
			float32_t sqrMag = SqrMagnitude();
			if (sqrMag == 1)
				return *this;

			uint32_t i;
			float32_t halfSqrMag = sqrMag * 0.5f, invMag = sqrMag;
			const float32_t threeHalves = 1.5f;

			memcpy(&i, &invMag, sizeof(uint32_t));
			i = 0x5f3759df - (i >> 1);
			memcpy(&invMag, &i, sizeof(uint32_t));
			invMag = invMag * (threeHalves - (halfSqrMag * invMag * invMag));
			invMag = invMag * (threeHalves - (halfSqrMag * invMag * invMag));

			Vector3 newVec{ x * invMag, y * invMag, z * invMag };
			return newVec;
		}
		/// @brief Returns the dot product between two vectors.
		/// @param other The other vector.
		float32_t Dot(const Vector3& other) const { return x * x + y * y + z * z; }
		/// @brief Returns the dot product between two vectors.
		/// @param other The other vector.
		float32_t Dot(Vector3&& other) const { return x * x + y * y + z * z; }
		/// @brief Returns the cross product between two vectors.
		/// @param other The other vector.
		Vector3 Cross(const Vector3& other) const { return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x }; }
		/// @brief Returns the cross product between two vectors.
		/// @param other The other vector.
		Vector3 Cross(Vector3&& other) const { return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x }; }

		~Vector3() = default;
	};
}