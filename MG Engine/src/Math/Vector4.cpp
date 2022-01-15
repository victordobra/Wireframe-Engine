#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "EngineMath.h"

namespace mge {
	void Vector4::Normalize() {
		float sqrMag = SqrMagnitude();
		if (sqrMag == 1 || sqrMag == 0)
			return;
		float mag = Sqrt(sqrMag);
		x /= mag;
		y /= mag;
		z /= mag;
		w /= mag;
	}
	Vector4 Vector4::Normalized() const {
		float sqrMag = SqrMagnitude();
		if (sqrMag == 1 || sqrMag == 0)
			return *this;
		Vector4 vec;
		float mag = Sqrt(sqrMag);
		vec.x /= mag;
		vec.y /= mag;
		vec.z /= mag;
		vec.w /= mag;
		return vec;
	}
	void Vector4::ClampMagnitude(float min, float max) {
		float sqrMag = SqrMagnitude();

		if (sqrMag < min * min) {
			float mag = Sqrt(sqrMag);
			float ratio = min / mag;
			x *= ratio;
			y *= ratio;
			z *= ratio;
			w *= ratio;
		}
		else if (sqrMag > max * max) {
			float mag = Sqrt(sqrMag);
			float ratio = max / mag;
			x *= ratio;
			y *= ratio;
			z *= ratio;
			w *= ratio;
		}
	}

	Vector4::Vector4(const Vector2& other) : x(other.x), y(other.y), z(0), w(0) { }
	Vector4::Vector4(Vector2&& other) : x(other.x), y(other.y), z(0), w(0) { }
	Vector4::Vector4(const Vector3& other) : x(other.x), y(other.y), z(other.z), w(0) { }
	Vector4::Vector4(Vector3&& other) : x(other.x), y(other.y), z(other.z), w(0) { }

	float Vector4::Dot(Vector4 x, Vector4 y) {
		return x.x * y.x + y.x * y.y + x.z * y.z;
	}
	Vector4 Vector4::Lerp(Vector4 a, Vector4 b, float t) {
		return a + (b - a) * t;
	}
}