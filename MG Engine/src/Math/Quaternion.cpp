#include "Quaternion.h"
#include "Vector3.h"
#include "Vector4.h"

namespace mge {
	inline Quaternion::Quaternion(Vector3 v, float w) : x(v.x), y(v.y), z(v.z), w(w) { }

	inline void Quaternion::Normalize() {
		float sqrMag = SqrMagnitude();
		if (sqrMag == 1 || sqrMag == 0)
			return;
		float mag = Sqrt(sqrMag);
		x /= mag; 
		y /= mag; 
		z /= mag; 
		w /= mag;
	}
	inline Quaternion Quaternion::Normalized() const {
		float sqrMag = SqrMagnitude();
		if (sqrMag == 1 || sqrMag == 0)
			return *this;
		Quaternion q;
		float mag = Sqrt(sqrMag);
		q.x /= mag; 
		q.y /= mag; 
		q.z /= mag; 
		q.w /= mag;
		return q;
	}

	inline Quaternion Quaternion::operator*(const Quaternion& other) const {
		return { w * other.x + x * other.w + y * other.z - z * other.y,
				 w * other.y - x * other.z + y * other.w + z * other.x,
				 w * other.z + x * other.y - y * other.x + z * other.w,
				 w * other.w - x * other.x - y * other.y - z * other.z };
	}
	inline Quaternion Quaternion::operator*(Quaternion&& other) const {
		return { w * other.x + x * other.w + y * other.z - z * other.y,
				 w * other.y - x * other.z + y * other.w + z * other.x,
				 w * other.z + x * other.y - y * other.x + z * other.w,
				 w * other.w - x * other.x - y * other.y - z * other.z };
	}

	Quaternion::Quaternion(const Vector3& other) : x(other.x), y(other.y), z(other.z), w(0) { }
	Quaternion::Quaternion(Vector3&& other) : x(other.x), y(other.y), z(other.z), w(0) { }
	Quaternion::Quaternion(const Vector4& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }
	Quaternion::Quaternion(Vector4&& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }

	Quaternion Quaternion::EulerAngles(float x, float y, float z) {
		return AroundAxis(z, Vector3(0, 0, 1)) * AroundAxis(x, Vector3(1, 0, 0)) * AroundAxis(y, Vector3(0, 1, 0));
	}
	Quaternion Quaternion::EulerAngles(Vector3 eulerAngles) {
		return AroundAxis(eulerAngles.z, Vector3(0, 0, 1)) * AroundAxis(eulerAngles.x, Vector3(1, 0, 0)) * AroundAxis(eulerAngles.y, Vector3(0, 1, 0));
	}
	Quaternion Quaternion::AroundAxis(float angle, Vector3 axis) {
		float sqrMag = axis.SqrMagnitude();
		if (sqrMag == 0)
			return { };
		if (sqrMag == 1)
			return Quaternion(axis * Sin(angle / 2 * degToRad), Cos(angle / 2 * degToRad));

		return Quaternion(axis / Sqrt(sqrMag) * Sin(angle / 2 * degToRad), Cos(angle / 2 * degToRad));
	}
}