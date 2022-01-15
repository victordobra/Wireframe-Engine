#pragma once
#include "EngineMath.h"

namespace mge {
	class Vector2;
	class Vector4;

	class Vector3 {
	public:
		float x{}, y{}, z{};

		inline Vector3() = default;
		inline Vector3(const Vector3&) = default;
		inline Vector3(Vector3&&) noexcept = default;
		inline Vector3(float x, float y) : x(x), y(y) { }
		inline Vector3(float x, float y, float z) : x(x), y(y), z(z) { }

		inline void Normalize();
		inline Vector3 Normalized() const;
		inline float Magnitude() const { return Sqrt(x * x + y * y + z * z); }
		inline float SqrMagnitude() const { return x * x + y * y + z * z; }
		inline void ClampMagnitude(float min, float max);

		inline Vector3 operator+(const Vector3& other) const { return { x + other.x, y + other.y, z + other.z }; }
		inline Vector3 operator+(Vector3&& other) const { return { x + other.x, y + other.y, z + other.z }; }
		inline Vector3 operator-(const Vector3& other) const { return { x - other.x, y - other.y, z - other.z }; }
		inline Vector3 operator-(Vector3&& other) const { return { x - other.x, y - other.y, z - other.z }; }
		inline Vector3 operator-() const { return { -x, -y, -z }; }
		inline Vector3 operator*(float other) const { return { x * other, y * other, z * other }; }
		inline Vector3 operator/(float other) const { return { x / other, y / other, z / other }; }

		inline Vector3& operator=(const Vector3& other) = default;
		inline Vector3& operator=(Vector3&& other) noexcept = default;
		inline Vector3& operator+=(const Vector3& other) { x += other.x; y += other.y; z += other.z; }
		inline Vector3& operator+=(Vector3&& other) noexcept { x += other.x; y += other.y; z += other.z; }
		inline Vector3& operator-=(const Vector3& other) { x -= other.x; y -= other.y; z -= other.z; }
		inline Vector3& operator-=(Vector3&& other) noexcept { x -= other.x; y -= other.y; z -= other.z; }
		inline Vector3& operator*=(float other) { x *= other; y /= other; z *= other; }
		inline Vector3& operator/=(float other) { x *= other; y /= other; z /= other;}

		inline bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z; }
		inline bool operator==(Vector3&& other) const { return x == other.x && y == other.y && z == other.z; }
		inline bool operator!=(const Vector3& other) const { return x != other.x || y != other.y || z != other.z; }
		inline bool operator!=(Vector3&& other) const { return x != other.x || y != other.y || z != other.z; }

		inline Vector3(const Vector2& other);
		inline Vector3(Vector2&& other);
		inline explicit Vector3(const Vector4& other);
		inline explicit Vector3(Vector4&& other);

		inline static float Dot(Vector3 a, Vector3 b);
		inline static Vector3 Lerp(Vector3 a, Vector3 b, float t);

		~Vector3() = default;
	};
}