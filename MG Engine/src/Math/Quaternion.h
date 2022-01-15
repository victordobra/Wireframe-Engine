#pragma once
#include "EngineMath.h"

namespace mge {
	class Vector3;
	class Vector4;

	class Quaternion {
	public:
		float x{0}, y{0}, z{0}, w{0};

		inline Quaternion() = default;
		inline Quaternion(const Quaternion&) = default;
		inline Quaternion(Quaternion&&) noexcept = default;
		inline Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
		inline Quaternion(Vector3 v, float w);

		inline void Normalize();
		inline Quaternion Normalized() const;
		inline void Invert() { x = -x; y = -y; z = -z; }
		inline Quaternion Inverted() const { return { -x, -y, -z, w }; }
		inline float Magnitude() const { return Sqrt(x * x + y * y + z * z + w * w); }
		inline float SqrMagnitude() const { return x * x + y * y + z * z + w * w; }

		inline Quaternion operator*(const Quaternion& other) const;
		inline Quaternion operator*(Quaternion&& other) const;
		inline Quaternion operator/(const Quaternion& other) const { return operator*(other.Inverted()); }
		inline Quaternion operator/(Quaternion&& other) const { return operator*(other.Inverted()); }

		inline Quaternion& operator=(const Quaternion& other) = default;
		inline Quaternion& operator=(Quaternion&& other) noexcept = default;
		inline Quaternion& operator*=(const Quaternion& other) { *this = operator*(other); return *this; }
		inline Quaternion& operator*=(Quaternion&& other) noexcept { *this = operator*(other); return *this; }
		inline Quaternion& operator/=(const Quaternion& other) { *this = operator/(other); return *this; }
		inline Quaternion& operator/=(Quaternion&& other) noexcept { *this = operator/(other); return *this; }

		inline bool operator==(const Quaternion& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		inline bool operator==(Quaternion&& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		inline bool operator!=(const Quaternion& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }
		inline bool operator!=(Quaternion&& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }

		inline explicit Quaternion(const Vector3& other);
		inline explicit Quaternion(Vector3&& other);
		inline explicit Quaternion(const Vector4& other);
		inline explicit Quaternion(Vector4&& other);

		static Quaternion EulerAngles(float x, float y, float z);
		static Quaternion EulerAngles(Vector3 eulerAngles);
		static Quaternion AroundAxis(float angle, Vector3 axis);

		~Quaternion() = default;
	};
}