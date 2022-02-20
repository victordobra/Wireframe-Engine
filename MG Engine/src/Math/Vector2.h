#pragma once
#include "Defines.h"

#include <cstring>

namespace mge {
	struct Vector2 {
		/// @brief Returns a vector with every element set as 0.
		static Vector2 Zero()  { return {  0.f,  0.f }; }
		/// @brief Returns a vector with every element set as 1.
		static Vector2 One()   { return {  1.f,  1.f }; }
		/// @brief Returns a vector that represents the up direction.
		static Vector2 Up()    { return {  0.f,  1.f }; }
		/// @brief Returns a vector that represents the down direction.
		static Vector2 Down()  { return {  0.f, -1.f }; }
		/// @brief Returns a vector that represents the left direction.
		static Vector2 Left()  { return { -1.f,  0.f }; }
		/// @brief Returns a vector that represents the right direction.
		static Vector2 Right() { return {  1.f,  0.f }; }

		union {
			float32_t elements[2]{ 0.f, 0.f };

			struct {
				union {
					float32_t x, r, s, u;
				};
				union {
					float32_t y, g, t, v;
				};
			};
		};

		Vector2() = default;
		Vector2(const Vector2&) = default;
		Vector2(Vector2&&) noexcept = default;
		Vector2(float32_t x) : x(x), y(x) { }
		Vector2(float32_t x, float32_t y) : x(x), y(y) { }

		Vector2& operator=(const Vector2&) = default;
		Vector2& operator=(Vector2&&) noexcept = default;

		bool8_t operator==(const Vector2& other) const { return x == other.x && y == other.y; }
		bool8_t operator==(Vector2&& other)      const { return x == other.x && y == other.y; }
		bool8_t operator!=(const Vector2& other) const { return x != other.x || y != other.y; }
		bool8_t operator!=(Vector2&& other)      const { return x != other.x || y != other.y; }

		Vector2 operator+(const Vector2& other)  const { return { x + other.x, y + other.y }; }
		Vector2 operator+(Vector2&& other)       const { return { x + other.x, y + other.y }; }
		Vector2 operator-(const Vector2& other)  const { return { x - other.x, y - other.y }; }
		Vector2 operator-(Vector2&& other)       const { return { x - other.x, y - other.y }; }
		Vector2 operator-()                      const { return { -x         , -y          }; }
		Vector2 operator*(const Vector2& other)  const { return { x * other.x, y * other.y }; }
		Vector2 operator*(Vector2&& other)       const { return { x * other.x, y * other.y }; }
		Vector2 operator*(float32_t other)       const { return { x * other  , y * other   }; }
		Vector2 operator/(const Vector2& other)  const { return { x / other.x, y / other.y }; }
		Vector2 operator/(Vector2&& other)       const { return { x / other.x, y / other.y }; }
		Vector2 operator/(float32_t other)       const { return { x / other  , y / other   }; }

		Vector2& operator+=(const Vector2& other)      { x += other.x; y += other.y; return *this; }
		Vector2& operator+=(Vector2&& other) noexcept  { x += other.x; y += other.y; return *this; }
		Vector2& operator-=(const Vector2& other)      { x -= other.x; y -= other.y; return *this; }
		Vector2& operator-=(Vector2&& other) noexcept  { x -= other.x; y -= other.y; return *this; }
		Vector2& operator*=(const Vector2& other)      { x *= other.x; y *= other.y; return *this; }
		Vector2& operator*=(Vector2&& other) noexcept  { x *= other.x; y *= other.y; return *this; }
		Vector2& operator*=(float32_t other)           { x *= other;   y *= other;   return *this; }
		Vector2& operator/=(const Vector2& other)      { x /= other.x; y /= other.y; return *this; }
		Vector2& operator/=(Vector2&& other) noexcept  { x /= other.x; y /= other.y; return *this; }
		Vector2& operator/=(float32_t other)           { x /= other;   y /= other;   return *this; }

		/// @brief Returns the length of the vector.
		float32_t Magnitude() const { return sqrtf(x * x + y * y); }
		/// @brief Returns the square of the length of the vector.
		float32_t SqrMagnitude() const { return x * x + y * y; }
		/// @brief Reduces the vector's length to 1.
		Vector2& Normalize() {
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

			return *this;
		} 
		/// @brief Returns a version of the vector with the length 1.
		Vector2 Normalized() const {
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

			Vector2 newVec{ x * invMag, y * invMag };
			return newVec;
		}
		/// @brief Returns the dot product between two vectors.
		/// @param other The other vector.
		Vector2 Dot(const Vector2& other) const { return x * other.x + y * other.y; }
		/// @brief Returns the dot product between two vectors.
		/// @param other The other vector.
		Vector2 Dot(Vector2&& other) const { return x * other.x + y * other.y; }

		~Vector2() = default;
	};
}