#pragma once
#include "EngineMath.h"

namespace mge {
	class Vector3;
	class Vector4;

	class Quaternion {
	public:
		float x{0}, y{0}, z{0}, w{1};

		Quaternion() = default;
		Quaternion(const Quaternion&) = default;
		Quaternion(Quaternion&&) noexcept = default;
		Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
		Quaternion(Vector3 v, float w);

		void Normalize();
		Quaternion Normalized() const;
		void Invert() { x = -x; y = -y; z = -z; }
		Quaternion Inverted() const { return { -x, -y, -z, w }; }
		float Magnitude() const { return Sqrt(x * x + y * y + z * z + w * w); }
		float SqrMagnitude() const { return x * x + y * y + z * z + w * w; }

		Quaternion operator*(const Quaternion& other) const;
		Quaternion operator*(Quaternion&& other) const;
		Quaternion operator/(const Quaternion& other) const { return operator*(other.Inverted()); }
		Quaternion operator/(Quaternion&& other) const { return operator*(other.Inverted()); }

		Quaternion& operator=(const Quaternion& other) = default;
		Quaternion& operator=(Quaternion&& other) noexcept = default;
		Quaternion& operator*=(const Quaternion& other) { *this = operator*(other); return *this; }
		Quaternion& operator*=(Quaternion&& other) noexcept { *this = operator*(other); return *this; }
		Quaternion& operator/=(const Quaternion& other) { *this = operator/(other); return *this; }
		Quaternion& operator/=(Quaternion&& other) noexcept { *this = operator/(other); return *this; }

		bool operator==(const Quaternion& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		bool operator==(Quaternion&& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		bool operator!=(const Quaternion& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }
		bool operator!=(Quaternion&& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }

		explicit Quaternion(const Vector3& other);
		explicit Quaternion(Vector3&& other);
		explicit Quaternion(const Vector4& other);
		explicit Quaternion(Vector4&& other);

		static Quaternion EulerAngles(float x, float y, float z);
		static Quaternion EulerAngles(Vector3 eulerAngles);
		static Quaternion AroundAxis(float angle, Vector3 axis);

		~Quaternion() = default;
	};
}