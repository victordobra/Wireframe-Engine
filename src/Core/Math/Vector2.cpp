#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include <math.h>

namespace wfe {
	// Constants
	const Vector2 Vector2::ZERO  {  0.0f,  0.0f };
	const Vector2 Vector2::ONE   {  1.0f,  1.0f };
	const Vector2 Vector2::UP    {  0.0f,  1.0f };
	const Vector2 Vector2::DOWN  {  0.0f, -1.0f };
	const Vector2 Vector2::LEFT  { -1.0f,  0.0f };
	const Vector2 Vector2::RIGHT {  1.0f,  0.0f };

	// Public functions
	Vector2::Vector2() : x(0.0f), y(0.0f) { }
	Vector2::Vector2(float x) : x(x), y(x) { }
	Vector2::Vector2(float x, float y) : x(x), y(y) { }
	Vector2::Vector2(const float elements[2]) : x(elements[0]), y(elements[1]) { }

	Vector2::Vector2(const Vector3& other) : x(other.x), y(other.y) { }
	Vector2::Vector2(const Vector4& other) : x(other.x), y(other.y) { }

	bool Vector2::operator==(const Vector2& other) const {
		// Compare each element
		return x == other.x && y == other.y;
	}
	bool Vector2::operator!=(const Vector2& other) const {
		// Compare each element
		return x != other.x || y != other.y;
	}

	Vector2 Vector2::operator+(const Vector2& other) const {
		// Calculate the resulting vector's dimensions
		return { x + other.x, y + other.y };
	}
	Vector2 Vector2::operator-(const Vector2& other) const {
		// Calculate the resulting vector's dimensions
		return { x - other.x, y - other.y };
	}
	Vector2 Vector2::operator-() const {
		// Calculate the resulting vector's dimensions
		return { -x, -y };
	}
	Vector2 Vector2::operator*(const Vector2& other) const {
		// Calculate the resulting vector's dimensions
		return { x * other.x, y * other.y };
	}
	Vector2 Vector2::operator/(const Vector2& other) const {
		// Calculate the resulting vector's dimensions
		return { x / other.x, y / other.y };
	}

	Vector2& Vector2::operator+=(const Vector2& other) {
		// Increase each element
		x += other.x;
		y += other.y;

		return *this;
	}
	Vector2& Vector2::operator-=(const Vector2& other) {
		// Decrease each element
		x -= other.x;
		y -= other.y;

		return *this;
	}
	Vector2& Vector2::operator*=(const Vector2& other) {
		// Multiply each element
		x *= other.x;
		y *= other.y;

		return *this;
	}
	Vector2& Vector2::operator*=(float other) {
		// Multiply each element
		x *= other;
		y *= other;

		return *this;
	}
	Vector2& Vector2::operator/=(const Vector2& other) {
		// Divide each element
		x /= other.x;
		y /= other.y;

		return *this;
	}
	Vector2& Vector2::operator/=(float other) {
		// Calculate the inverse of the given value
		float invOther = 1.0f / other;

		// Multiply each element with the inverse of the given value
		x *= invOther;
		y *= invOther;

		return *this;
	}

	float Vector2::Magnitude() const {
		// Calculate the Vector2's magnitude
		return sqrt(x * x + y * y);
	}
	float Vector2::SqrMagnitude() const {
		// Calculate the Vector2's square magnitude
		return x * x + y * y;
	}
	float Vector2::InvMagnitude() const {
		// Calculate the Vector2's inverse magnitude using the fast inverse square root function
		return 1.0f / sqrt(x * x + y * y);
	}

	Vector2& Vector2::Normalize() {
		// Calculate the Vector2's inverse magnitude
		float invMag = InvMagnitude();

		// Multiply each element by the inverse magnitude
		x *= invMag;
		y *= invMag;

		return *this;
	}
	Vector2 Vector2::Normalized() const {
		// Calculate the Vector2's inverse magnitude
		float invMag = InvMagnitude();

		// Calculate the resulting Vector2's elements
		return { x * invMag, y * invMag };
	}
	Vector2& Vector2::Flip() {
		// Set each element to its opposite
		x = -x;
		y = -y;

		return *this;
	}
	Vector2 Vector2::Flipped() const {
		// Calculate the resulting Vector2's elements
		return { -x, -y };
	}

	float Vector2::Dot(const Vector2& other) const {
		// Calculate the dot product
		return x * other.x + y * other.y;
	}
	float Vector2::Angle(const Vector2& other) const {
		// Calculate the dot product
		float dot = Dot(other);

		// Calculate the cosine of the angle using the dot product
		float angleCos = dot * InvMagnitude() * other.InvMagnitude();

		// Calculate the angle between the two Vector2s 
		return acosf(angleCos);
	}

	Vector2 operator*(const Vector2& vec, float val) {
		// Calculate the resulting vector's dimensions
		return { vec.x * val, vec.y * val };
	}
	Vector2 operator*(float val, const Vector2& vec) {
		// Calculate the resulting vector's dimensions
		return { vec.x * val, vec.y * val };
	}
	Vector2 operator/(const Vector2& vec, float val) {
		// Calculate the inverse of the given value
		float invVal = 1.0f / val;

		// Calculate the resulting vector's dimensions
		return { vec.x * invVal, vec.y * invVal };
	}
	Vector2 operator/(float val, const Vector2& vec) {
		// Calculate the resulting vector's dimensions
		return { val / vec.x, val / vec.y };
	}
}