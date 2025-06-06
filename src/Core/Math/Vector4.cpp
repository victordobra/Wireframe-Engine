#include "Vector4.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"
#include <math.h>

namespace wfe {
	// Constants
	const Vector4 Vector4::ZERO { 0.0f, 0.0f, 0.0f, 0.0f };
	const Vector4 Vector4::ONE  { 1.0f, 1.0f, 1.0f, 1.0f };

	// Public functions
	Vector4::Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0) { }
	Vector4::Vector4(float x) : x(x), y(x), z(x), w(x) { }
	Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
	Vector4::Vector4(const float elements[4]) : x(elements[0]), y(elements[1]), z(elements[2]), w(elements[3]) { }

	Vector4::Vector4(const Vector2& other) : x(other.x), y(other.y), z(0.0f), w(0.0f) { }
	Vector4::Vector4(const Vector3& other) : x(other.x), y(other.y), z(other.z), w(0.0f) { }
	Vector4::Vector4(const Quaternion& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }

	bool Vector4::operator==(const Vector4& other) const {
		// Compare each element
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}
	bool Vector4::operator!=(const Vector4& other) const {
		// Compare each element
		return x != other.x || y != other.y || z != other.z || w != other.w;
	}

	Vector4 Vector4::operator+(const Vector4& other) const {
		// Calculate the resulting vector's dimensions
		return { x + other.x, y + other.y, z + other.z, w + other.w };
	}
	Vector4 Vector4::operator-(const Vector4& other) const {
		// Calculate the resulting vector's dimensions
		return { x - other.x, y - other.y, z - other.z, w + other.w };
	}
	Vector4 Vector4::operator-() const {
		// Calculate the resulting vector's dimensions
		return { -x, -y, -z, -w };
	}
	Vector4 Vector4::operator*(const Vector4& other) const {
		// Calculate the resulting vector's dimensions
		return { x * other.x, y * other.y, z * other.z, w * other.w };
	}
	Vector4 Vector4::operator/(const Vector4& other) const {
		// Calculate the resulting vector's dimensions
		return { x / other.x, y / other.y, z / other.z, w / other.w };
	}

	Vector4& Vector4::operator+=(const Vector4& other) {
		// Increase each element
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;

		return *this;
	}
	Vector4& Vector4::operator-=(const Vector4& other) {
		// Decrease each element
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;

		return *this;
	}
	Vector4& Vector4::operator*=(const Vector4& other) {
		// Multiply each element
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;

		return *this;
	}
	Vector4& Vector4::operator*=(float other) {
		// Multiply each element
		x *= other;
		y *= other;
		z *= other;
		w *= other;

		return *this;
	}
	Vector4& Vector4::operator/=(const Vector4& other) {
		// Divide each element
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;

		return *this;
	}
	Vector4& Vector4::operator/=(float other) {
		// Calculate the inverse of the given value
		float invOther = 1.0f / other;

		// Multiply each element with the inverse of the given value
		x *= invOther;
		y *= invOther;
		z *= invOther;
		w *= invOther;

		return *this;
	}

	float Vector4::Magnitude() const {
		// Calculate the Vector4's magnitude
		return sqrt(x * x + y * y + z * z + w * w);
	}
	float Vector4::SqrMagnitude() const {
		// Calculate the Vector4's square magnitude
		return x * x + y * y + z * z + w * w;
	}
	float Vector4::InvMagnitude() const {
		// Calculate the Vector4's inverse magnitude
		return 1.0f / sqrt(x * x + y * y + z * z + w * w);
	}

	Vector4& Vector4::Normalize() {
		// Calculate the Vector4's inverse magnitude
		float invMag = InvMagnitude();

		// Multiply each element by the inverse magnitude
		x *= invMag;
		y *= invMag;
		z *= invMag;
		w *= invMag;

		return *this;
	}
	Vector4 Vector4::Normalized() const {
		// Calculate the Vector4's inverse magnitude
		float invMag = InvMagnitude();

		// Calculate the resulting Vector4's elements
		return { x * invMag, y * invMag, z * invMag, w * invMag };
	}
	Vector4& Vector4::Flip() {
		// Set each element to its opposite
		x = -x;
		y = -y;
		z = -z;
		w = -w;

		return *this;
	}
	Vector4 Vector4::Flipped() const {
		// Calculate the resulting Vector4's elements
		return { -x, -y, -z, -w };
	}

	float Vector4::Dot(const Vector4& other) const {
		// Calculate the dot product
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}
	float Vector4::Angle(const Vector4& other) const {
		// Calculate the dot product
		float dot = Dot(other);

		// Calculate the cosine of the angle using the dot product
		float angleCos = dot * InvMagnitude() * other.InvMagnitude();

		// Calculate the angle between the two Vector4s 
		return acosf(angleCos);
	}

	Vector4 operator*(const Vector4& vec, float val) {
		// Calculate the resulting vector's dimensions
		return { vec.x * val, vec.y * val, vec.z * val, vec.w * val };
	}
	Vector4 operator*(float val, const Vector4& vec) {
		// Calculate the resulting vector's dimensions
		return { vec.x * val, vec.y * val, vec.z * val, vec.w * val };
	}
	Vector4 operator/(const Vector4& vec, float val) {
		// Calculate the inverse of the given value
		float invVal = 1.0f / val;

		// Calculate the resulting vector's dimensions
		return { vec.x * invVal, vec.y * invVal, vec.z * invVal, vec.w * invVal };
	}
	Vector4 operator/(float val, const Vector4& vec) {
		// Calculate the resulting vector's dimensions
		return { val / vec.x, val / vec.y, val / vec.z, val / vec.w };
	}
}