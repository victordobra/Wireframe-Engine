#include "Quaternion.hpp"
#include <math.h>

namespace wfe {
	// Constants
	const Quaternion Quaternion::IDENTITY { 0.0f, 0.0f, 0.0f, 1.0f };

	// Public functions
	Quaternion::Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) { }
	Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { } 
	Quaternion::Quaternion(const float elements[4]) : x(elements[0]), y(elements[1]), z(elements[2]), w(elements[3]) { }

	Quaternion::Quaternion(const Vector4& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }

	bool Quaternion::operator==(const Quaternion& other) const {
		// Compare the two quaternion's elements
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}
	bool Quaternion::operator!=(const Quaternion& other) const {
		// Compare the two quaternion's elements
		return x != other.x || y != other.y || z != other.z || w != other.w;
	}

	Quaternion Quaternion::operator*(const Quaternion& other) const {
		// Calculate the resulting quaternion's elements
		return {  x * other.w + y * other.z - z * other.y + w * other.x,
		         -x * other.z + y * other.w + z * other.x + w * other.y,
		          x * other.y - y * other.x + z * other.w + w * other.z,
		         -x * other.x - y * other.y - z * other.z + w * other.w  };
	}
	Quaternion& Quaternion::operator*=(const Quaternion& other) {
		// Save the quaternion's current elements
		float x = this->x, y = this->y, z = this->z, w = this->w;

		// Set its new elements
		this->x =  x * other.w + y * other.z - z * other.y + w * other.x;
		this->y = -x * other.z + y * other.w + z * other.x + w * other.y;
		this->z =  x * other.y - y * other.x + z * other.w + w * other.z;
		this->w = -x * other.x - y * other.y - z * other.z + w * other.w;

		return *this;
	}

	float Quaternion::Magnitude() const {
		// Calculate the quaternion's magnitude
		return sqrtf(x * x + y * y + z * z + w * w);
	}
	float Quaternion::SqrMagnitude() const {
		// Calculate the quaternion's magnitude
		return x * x + y * y + z * z + w * w;
	}
	float Quaternion::InvMagnitude() const {
		// Calculate the quaternion's inverse magnitude
		return 1 / sqrtf(x * x + y * y + z * z + w * w);
	}

	Quaternion& Quaternion::Normalize() {
		// Calculate the quaternion's inverse magnitude
		float invMag = InvMagnitude();

		// Multiply every element by the inverse magnitude
		x *= invMag;
		y *= invMag;
		z *= invMag;
		w *= invMag;

		return *this;
	}
	Quaternion Quaternion::Normalized() const {
		// Calculate the quaternion's inverse magnitude
		float invMag = InvMagnitude();

		// Calculate the resulting quaternion's elements
		return { x * invMag, y * invMag, z * invMag, w * invMag };
	}
	Quaternion& Quaternion::Invert() {
		// Invert every element of the quaternion
		x = -x;
		y = -y;
		z = -z;
		w = -w;

		return *this;
	}
	Quaternion Quaternion::Inverted() const {
		// Calculate the resulting quaternion's elements
		return { -x, -y, -z, -w };
	}

	float Quaternion::Dot(const Quaternion& other) const {
		// Calculate the two quaternions' dot product
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	Quaternion Quaternion::AroundAxis(Vector3 axis, float angle) {
		// Calculate the sine and cosine of the given angle
		float halfAngle = angle * 0.5f;
		float halfSin = sinf(halfAngle), halfCos = cosf(halfAngle);

		// Calculate the resulting quaternion's coordinates
		return { axis.x * halfSin, axis.y * halfSin, axis.z * halfSin, halfCos };
	}
	Quaternion Quaternion::EulerAngles(Vector3 eulerAngles) {
		// Calculate the halves of all euler angles
		float halfX = eulerAngles.x * 0.5f;
		float halfY = eulerAngles.y * 0.5f;
		float halfZ = eulerAngles.z * 0.5f;

		// Calculate the sines and cosines of the angle halves
		float sinX = sinf(halfX), cosX = cosf(halfX);
		float sinY = sinf(halfY), cosY = cosf(halfY);
		float sinZ = sinf(halfZ), cosZ = cosf(halfZ);

		// Calculate the elements of the resulting quaternion
		return {  sinX * cosY * cosZ + cosX * sinY * sinZ,
		         -sinX * cosY * sinZ + cosX * sinY * cosZ,
		         -sinX * sinY * cosZ + cosX * cosY * sinZ,
		          sinX * sinY * sinZ + cosX * cosY * cosZ  };
	}
}