#pragma once
#include "EngineMath.h"

namespace mge {
	class Vector3;
	class Vector4;

	class Vector2 {
	public:
		float x{}, y{};

		inline Vector2() = default;
		inline Vector2(const Vector2&) = default;
		inline Vector2(Vector2&&) noexcept = default;
		inline Vector2(float x, float y) : x(x), y(y) { }

		inline void Normalize();
		inline Vector2 Normalized() const;
		inline float Magnitude() const { return Sqrt(x * x + y * y); }
		inline float SqrMagnitude() const { return x * x + y * y; }
		inline void ClampMagnitude(float min, float max);

		inline Vector2 operator+(const Vector2& other) const { return { x + other.x, y + other.y }; }
		inline Vector2 operator+(Vector2&& other) const { return { x + other.x, y + other.y }; }
		inline Vector2 operator-(const Vector2& other) const { return { x - other.x, y - other.y }; }
		inline Vector2 operator-(Vector2&& other) const { return { x - other.x, y - other.y }; }
		inline Vector2 operator-() const { return { -x, -y }; }
		inline Vector2 operator*(float other) const { return { x * other, y * other }; }
		inline Vector2 operator/(float other) const { return { x / other, y / other }; }

		inline Vector2& operator=(const Vector2& other) = default;
		inline Vector2& operator=(Vector2&& other) noexcept = default;
		inline Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; }
		inline Vector2& operator+=(Vector2&& other) noexcept { x += other.x; y += other.y; }
		inline Vector2& operator-=(const Vector2& other) { x -= other.x; y -= other.y; }
		inline Vector2& operator-=(Vector2&& other) noexcept { x -= other.x; y -= other.y; }
		inline Vector2& operator*=(float other) { x *= other; y /= other; }
		inline Vector2& operator/=(float other) { x *= other; y /= other; }

		inline bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
		inline bool operator==(Vector2&& other) const { return x == other.x && y == other.y; }
		inline bool operator!=(const Vector2& other) const { return x != other.x || y != other.y; }
		inline bool operator!=(Vector2&& other) const { return x != other.x || y != other.y; }

		inline explicit Vector2(const Vector3& other);
		inline explicit Vector2(Vector3&& other);
		inline explicit Vector2(const Vector4& other);
		inline explicit Vector2(Vector4&& other);

		inline static float Dot(Vector2 a, Vector2 b);
		inline static Vector2 Lerp(Vector2 a, Vector2 b, float t);

		~Vector2() = default;
	};
}