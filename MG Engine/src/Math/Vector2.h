#pragma once
#include "EngineMath.h"

namespace mge {
	class Vector3;
	class Vector4;

	class Vector2 {
	public:
		float x{}, y{};

		Vector2() = default;
		Vector2(const Vector2&) = default;
		Vector2(Vector2&&) noexcept = default;
		Vector2(float x, float y) : x(x), y(y) { }

		void Normalize();
		Vector2 Normalized() const;
		float Magnitude() const { return Sqrt(x * x + y * y); }
		float SqrMagnitude() const { return x * x + y * y; }
		void ClampMagnitude(float min, float max);

		Vector2 operator+(const Vector2& other) const { return { x + other.x, y + other.y }; }
		Vector2 operator+(Vector2&& other) const { return { x + other.x, y + other.y }; }
		Vector2 operator-(const Vector2& other) const { return { x - other.x, y - other.y }; }
		Vector2 operator-(Vector2&& other) const { return { x - other.x, y - other.y }; }
		Vector2 operator-() const { return { -x, -y }; }
		Vector2 operator*(float other) const { return { x * other, y * other }; }
		Vector2 operator/(float other) const { return { x / other, y / other }; }

		Vector2& operator=(const Vector2& other) = default;
		Vector2& operator=(Vector2&& other) noexcept = default;
		Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; }
		Vector2& operator+=(Vector2&& other) noexcept { x += other.x; y += other.y; }
		Vector2& operator-=(const Vector2& other) { x -= other.x; y -= other.y; }
		Vector2& operator-=(Vector2&& other) noexcept { x -= other.x; y -= other.y; }
		Vector2& operator*=(float other) { x *= other; y /= other; }
		Vector2& operator/=(float other) { x *= other; y /= other; }

		bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
		bool operator==(Vector2&& other) const { return x == other.x && y == other.y; }
		bool operator!=(const Vector2& other) const { return x != other.x || y != other.y; }
		bool operator!=(Vector2&& other) const { return x != other.x || y != other.y; }

		explicit Vector2(const Vector3& other);
		explicit Vector2(Vector3&& other);
		explicit Vector2(const Vector4& other);
		explicit Vector2(Vector4&& other);

		static float Dot(Vector2 a, Vector2 b);
		static Vector2 Lerp(Vector2 a, Vector2 b, float t);

		~Vector2() = default;
	};
}