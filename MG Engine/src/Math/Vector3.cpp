#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "EngineMath.h"

namespace mge {
	void Vector3::Normalize() {
		float sqrMag = SqrMagnitude();
		if (sqrMag == 1 || sqrMag == 0)
			return;
		float mag = Sqrt(sqrMag);
		x /= mag;
		y /= mag;
		z /= mag;
	}
	Vector3 Vector3::Normalized() const {
		float sqrMag = SqrMagnitude();
		if (sqrMag == 1 || sqrMag == 0)
			return *this;
		Vector3 vec;
		float mag = Sqrt(sqrMag);
		vec.x /= mag;
		vec.y /= mag;
		vec.z /= mag;
		return vec;
	}
	void Vector3::ClampMagnitude(float min, float max) {
		float sqrMag = SqrMagnitude();

		if (sqrMag < min * min) {
			float mag = Sqrt(sqrMag);
			float ratio = min / mag;
			x *= ratio;
			y *= ratio;
			z *= ratio;
		}
		else if (sqrMag > max * max) {
			float mag = Sqrt(sqrMag);
			float ratio = max / mag;
			x *= ratio;
			y *= ratio;
			z *= ratio;
		}
	}

	Vector3::Vector3(const Vector2& other) : x(other.x), y(other.y), z(0) { }
	Vector3::Vector3(Vector2&& other) : x(other.x), y(other.y), z(0) { }
	Vector3::Vector3(const Vector4& other) : x(other.x), y(other.y), z(other.z) { }
	Vector3::Vector3(Vector4&& other) : x(other.x), y(other.y), z(other.z) { }

	float Vector3::Dot(Vector3 x, Vector3 y) {
		return x.x * y.x + y.x * y.y + x.z * y.z;
	}
	Vector3 Vector3::Lerp(Vector3 a, Vector3 b, float t) {
		return a + (b - a) * t;
	}
}