#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "EngineMath.h"

namespace mge {
	void Vector2::Normalize() {
		float sqrMag = SqrMagnitude();
		if (sqrMag == 1 || sqrMag == 0)
			return;
		float mag = Sqrt(sqrMag);
		x /= mag;
		y /= mag;
	}
	Vector2 Vector2::Normalized() const {
		float sqrMag = SqrMagnitude();
		if (sqrMag == 1 || sqrMag == 0)
			return *this;
		Vector2 vec;
		float mag = Sqrt(sqrMag);
		vec.x /= mag;
		vec.y /= mag;
		return vec;
	}
	void Vector2::ClampMagnitude(float min, float max) {
		float sqrMag = SqrMagnitude();

		if (sqrMag < min * min) {
			float mag = Sqrt(sqrMag);
			float ratio = min / mag;
			x *= ratio;
			y *= ratio;
		} else if (sqrMag > max * max) {
			float mag = Sqrt(sqrMag);
			float ratio = max / mag;
			x *= ratio;
			y *= ratio;
		}
	}

	Vector2::Vector2(const Vector3& other) : x(other.x), y(other.y) { }
	Vector2::Vector2(Vector3&& other) : x(other.x), y(other.y) { }
	Vector2::Vector2(const Vector4& other) : x(other.x), y(other.y) { }
	Vector2::Vector2(Vector4&& other) : x(other.x), y(other.y) { }
	
	float Vector2::Dot(Vector2 x, Vector2 y) {
		return x.x * y.x + y.x * y.y;
	}
	Vector2 Vector2::Lerp(Vector2 a, Vector2 b, float t) {
		return a + (b - a) * t;
	}
}