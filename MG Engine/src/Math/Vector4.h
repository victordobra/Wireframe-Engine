#pragma once
#include "EngineMath.h"

namespace mge {
	class Vector2;
	class Vector3;

	class Vector4 {
	public:
		float x{}, y{}, z{}, w{};

		inline Vector4() = default;
		inline Vector4(const Vector4&) = default;
		inline Vector4(Vector4&&) noexcept = default;
		inline Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

		inline void Normalize();
		inline Vector4 Normalized() const;
		inline float Magnitude() const { return Sqrt(x * x + y * y + z * z + w * w); }
		inline float SqrMagnitude() const { return x * x + y * y + z * z + w * w; }
		inline void ClampMagnitude(float min, float max);

		inline Vector4 operator+(const Vector4& other) const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
		inline Vector4 operator+(Vector4&& other) const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
		inline Vector4 operator-(const Vector4& other) const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }
		inline Vector4 operator-(Vector4&& other) const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }
		inline Vector4 operator-() const { return { -x, -y, -z, -w }; }
		inline Vector4 operator*(float other) const { return { x * other, y * other, z * other, w * other }; }
		inline Vector4 operator/(float other) const { return { x / other, y / other, z / other, w / other }; }

		inline Vector4& operator=(const Vector4& other) = default;
		inline Vector4& operator=(Vector4&& other) noexcept = default;
		inline Vector4& operator+=(const Vector4& other) { x += other.x; y += other.y; z += other.z; w += other.w; }
		inline Vector4& operator+=(Vector4&& other) noexcept { x += other.x; y += other.y; z += other.z; w += other.w; }
		inline Vector4& operator-=(const Vector4& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; }
		inline Vector4& operator-=(Vector4&& other) noexcept { x -= other.x; y -= other.y; z -= other.z; w -= other.w; }
		inline Vector4& operator*=(float other) { x *= other; y /= other; z *= other; w *= other; }
		inline Vector4& operator/=(float other) { x *= other; y /= other; z /= other; w /= other; }

		inline bool operator==(const Vector4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		inline bool operator==(Vector4&& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		inline bool operator!=(const Vector4& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }
		inline bool operator!=(Vector4&& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }

		inline Vector4(const Vector2& other);
		inline Vector4(Vector2&& other);
		inline Vector4(const Vector3& other);
		inline Vector4(Vector3&& other);

		inline static float Dot(Vector4 a, Vector4 b);
		inline static Vector4 Lerp(Vector4 a, Vector4 b, float t);

		~Vector4() = default;
	};
}