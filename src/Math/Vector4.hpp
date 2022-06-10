#pragma once

#include "Core.hpp"
#include <string.h>
#include <math.h>

namespace mge {
	struct Vector4 {
		/// @brief Returns a vector with every element set as 0.
		static Vector4 Zero() { return { 0.f, 0.f, 0.f, 0.f }; }
		/// @brief Returns a vector with every element set as 1.
		static Vector4 One()  { return { 1.f, 1.f, 1.f, 1.f }; }

		union {
			float32_t elements[4]{ 0.f, 0.f, 0.f, 0.f };

			struct {
				union {
					float32_t x, r, s;
				};
				union {
					float32_t y, g, t;
				};
				union {
					float32_t z, b, p;
				};
				union {
					float32_t w, a, q;
				};
			};
		};

		Vector4() = default;
		Vector4(const Vector4&) = default;
		Vector4(Vector4&&) noexcept = default;
		Vector4(float32_t x) : x(x), y(x), z(x), w(x) { }
		Vector4(float32_t x, float32_t y, float32_t z, float32_t w) : x(x), y(y), z(z), w(w) { }

		Vector4(const Vector2& other) : x(other.x), y(other.y), z(0), w(0) { }
		Vector4(Vector2&& other)      : x(other.x), y(other.y), z(0), w(0) { }
		Vector4(const Vector3& other) : x(other.x), y(other.y), z(other.z), w(0) { }
		Vector4(Vector3&& other)      : x(other.x), y(other.y), z(other.z), w(0) { }

		Vector4& operator=(const Vector4&) = default;
		Vector4& operator=(Vector4&&) noexcept = default;

		bool8_t operator==(const Vector4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		bool8_t operator==(Vector4&& other)      const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		bool8_t operator!=(const Vector4& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }
		bool8_t operator!=(Vector4&& other)      const { return x != other.x || y != other.y || z != other.z || w != other.w; }

		Vector4 operator+(const Vector4& other)  const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
		Vector4 operator+(Vector4&& other)       const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
		Vector4 operator-(const Vector4& other)  const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }
		Vector4 operator-(Vector4&& other)       const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }
		Vector4 operator-()                      const { return { -x         , -y         , -z         , -w          }; }
		Vector4 operator*(const Vector4& other)  const { return { x * other.x, y * other.y, z * other.z, w * other.w }; }
		Vector4 operator*(Vector4&& other)       const { return { x * other.x, y * other.y, z * other.z, w * other.w }; }
		Vector4 operator*(float32_t other)       const { return { x * other  , y * other  , z * other  , w * other   }; }
		Vector4 operator/(const Vector4& other)  const { return { x / other.x, y / other.y, z / other.z, w / other.w }; }
		Vector4 operator/(Vector4&& other)       const { return { x / other.x, y / other.y, z / other.z, w / other.w }; }
		Vector4 operator/(float32_t other)       const { return { x / other  , y / other  , z / other  , w / other   }; }

		Vector4& operator+=(const Vector4& other)      { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
		Vector4& operator+=(Vector4&& other) noexcept  { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
		Vector4& operator-=(const Vector4& other)      { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
		Vector4& operator-=(Vector4&& other) noexcept  { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
		Vector4& operator*=(const Vector4& other)      { x *= other.x; y *= other.y; z *= other.z; w *= other.w; return *this; }
		Vector4& operator*=(Vector4&& other) noexcept  { x *= other.x; y *= other.y; z *= other.z; w *= other.w; return *this; }
		Vector4& operator*=(float32_t other)           { x *= other;   y *= other;   z *= other;   w *= other;   return *this; }
		Vector4& operator/=(const Vector4& other)      { x /= other.x; y /= other.y; z /= other.z; w /= other.w; return *this; }
		Vector4& operator/=(Vector4&& other) noexcept  { x /= other.x; y /= other.y; z /= other.z; w /= other.w; return *this; }
		Vector4& operator/=(float32_t other)           { x /= other;   y /= other;   z /= other;   w /= other;   return *this; }

		/// @brief Returns the length of the vector.
		float32_t Magnitude() const { return sqrtf(x * x + y * y + z * z + w * w); }
		/// @brief Returns the square of the length of the vector.
		float32_t SqrMagnitude() const { return x * x + y * y + z * z, w * w; }
		/// @brief Reduces the vector's length to 1.
		Vector4& Normalize() {
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
			w *= invMag;

			return *this;
		}
		/// @brief Returns a version of the vector with the length 1.
		Vector4 Normalized() const {
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

			Vector4 newVec{ x * invMag, y * invMag, z * invMag, w * invMag };
			return newVec;
		}
		/// @brief Returns the dot product between two vectors.
		/// @param other The other vector.
		float32_t Dot(const Vector4& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }
		/// @brief Returns the dot product between two vectors.
		/// @param other The other vector.
		float32_t Dot(Vector4&& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }

		~Vector4() = default;
	};
}