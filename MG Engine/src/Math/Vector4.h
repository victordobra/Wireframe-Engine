#pragma once
#include "EngineMath.h"

namespace mge {
	class Vector2;
	class Vector3;

	class Vector4 {
	public:
		float x{}, y{}, z{}, w{};

		Vector4() = default;
		Vector4(const Vector4&) = default;
		Vector4(Vector4&&) noexcept = default;
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

		void Normalize();
		Vector4 Normalized() const;
		float Magnitude() const { return Sqrt(x * x + y * y + z * z + w * w); }
		float SqrMagnitude() const { return x * x + y * y + z * z + w * w; }
		void ClampMagnitude(float min, float max);

		Vector4 operator+(const Vector4& other) const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
		Vector4 operator+(Vector4&& other) const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
		Vector4 operator-(const Vector4& other) const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }
		Vector4 operator-(Vector4&& other) const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }
		Vector4 operator-() const { return { -x, -y, -z, -w }; }
		Vector4 operator*(float other) const { return { x * other, y * other, z * other, w * other }; }
		Vector4 operator/(float other) const { return { x / other, y / other, z / other, w / other }; }

		Vector4& operator=(const Vector4& other) = default;
		Vector4& operator=(Vector4&& other) noexcept = default;
		Vector4& operator+=(const Vector4& other) { x += other.x; y += other.y; z += other.z; w += other.w; }
		Vector4& operator+=(Vector4&& other) noexcept { x += other.x; y += other.y; z += other.z; w += other.w; }
		Vector4& operator-=(const Vector4& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; }
		Vector4& operator-=(Vector4&& other) noexcept { x -= other.x; y -= other.y; z -= other.z; w -= other.w; }
		Vector4& operator*=(float other) { x *= other; y /= other; z *= other; w *= other; }
		Vector4& operator/=(float other) { x *= other; y /= other; z /= other; w /= other; }

		bool operator==(const Vector4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		bool operator==(Vector4&& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		bool operator!=(const Vector4& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }
		bool operator!=(Vector4&& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }

		Vector4(const Vector2& other);
		Vector4(Vector2&& other);
		Vector4(const Vector3& other);
		Vector4(Vector3&& other);

		static float Dot(Vector4 a, Vector4 b);
		static Vector4 Lerp(Vector4 a, Vector4 b, float t);

		~Vector4() = default;
	};
}