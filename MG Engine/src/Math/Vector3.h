#pragma once
#include "EngineMath.h"

namespace mge {
	class Vector2;
	class Vector4;

	class Vector3 {
	public:
		float x{}, y{}, z{};

		Vector3() = default;
		Vector3(const Vector3&) = default;
		Vector3(Vector3&&) noexcept = default;
		Vector3(float x, float y) : x(x), y(y) { }
		Vector3(float x, float y, float z) : x(x), y(y), z(z) { }

		void Normalize();
		Vector3 Normalized() const;
		float Magnitude() const { return Sqrt(x * x + y * y + z * z); }
		float SqrMagnitude() const { return x * x + y * y + z * z; }
		void ClampMagnitude(float min, float max);

		Vector3 operator+(const Vector3& other) const { return { x + other.x, y + other.y, z + other.z }; }
		Vector3 operator+(Vector3&& other) const { return { x + other.x, y + other.y, z + other.z }; }
		Vector3 operator-(const Vector3& other) const { return { x - other.x, y - other.y, z - other.z }; }
		Vector3 operator-(Vector3&& other) const { return { x - other.x, y - other.y, z - other.z }; }
		Vector3 operator-() const { return { -x, -y, -z }; }
		Vector3 operator*(float other) const { return { x * other, y * other, z * other }; }
		Vector3 operator/(float other) const { return { x / other, y / other, z / other }; }

		Vector3& operator=(const Vector3& other) = default;
		Vector3& operator=(Vector3&& other) noexcept = default;
		Vector3& operator+=(const Vector3& other) { x += other.x; y += other.y; z += other.z; }
		Vector3& operator+=(Vector3&& other) noexcept { x += other.x; y += other.y; z += other.z; }
		Vector3& operator-=(const Vector3& other) { x -= other.x; y -= other.y; z -= other.z; }
		Vector3& operator-=(Vector3&& other) noexcept { x -= other.x; y -= other.y; z -= other.z; }
		Vector3& operator*=(float other) { x *= other; y /= other; z *= other; }
		Vector3& operator/=(float other) { x *= other; y /= other; z /= other;}

		bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z; }
		bool operator==(Vector3&& other) const { return x == other.x && y == other.y && z == other.z; }
		bool operator!=(const Vector3& other) const { return x != other.x || y != other.y || z != other.z; }
		bool operator!=(Vector3&& other) const { return x != other.x || y != other.y || z != other.z; }

		Vector3(const Vector2& other);
		Vector3(Vector2&& other);
		explicit Vector3(const Vector4& other);
		explicit Vector3(Vector4&& other);

		static float Dot(Vector3 a, Vector3 b);
		static Vector3 Lerp(Vector3 a, Vector3 b, float t);

		~Vector3() = default;
	};
}