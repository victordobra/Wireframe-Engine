#include "Vector3.hpp"
#include "Vector2.hpp"
#include "Vector4.hpp"
#include <math.h>

namespace wfe {
	// Constants
	const Vector3 Vector3::ZERO    {  0.0f,  0.0f,  0.0f };
	const Vector3 Vector3::ONE     {  1.0f,  1.0f,  1.0f };
	const Vector3 Vector3::UP      {  0.0f,  1.0f,  0.0f };
	const Vector3 Vector3::DOWN    {  0.0f, -1.0f,  0.0f };
	const Vector3 Vector3::LEFT    { -1.0f,  0.0f,  0.0f };
	const Vector3 Vector3::RIGHT   {  1.0f,  0.0f,  0.0f };
    const Vector3 Vector3::FORWARD {  0.0f,  0.0f, -1.0f };
    const Vector3 Vector3::BACK    {  0.0f,  0.0f,  1.0f };

	// Public functions
	Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) { }
	Vector3::Vector3(float x) : x(x), y(x), z(x) { }
	Vector3::Vector3(float x, float y) : x(x), y(y), z(0.0f) { }
	Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
	Vector3::Vector3(const float elements[3]) : x(elements[0]), y(elements[1]), z(elements[2]) { }

	Vector3::Vector3(const Vector2& other) : x(other.x), y(other.y), z(0.0f) { }
	Vector3::Vector3(const Vector4& other) : x(other.x), y(other.y), z(other.z) { }

	bool Vector3::operator==(const Vector3& other) const {
		// Compare each element
		return x == other.x && y == other.y && z == other.z;
	}
	bool Vector3::operator!=(const Vector3& other) const {
		// Compare each element
		return x != other.x || y != other.y || z != other.z;
	}

	Vector3 Vector3::operator+(const Vector3& other) const {
		// Calculate the resulting vector's dimensions
		return { x + other.x, y + other.y, z + other.z };
	}
	Vector3 Vector3::operator-(const Vector3& other) const {
		// Calculate the resulting vector's dimensions
		return { x - other.x, y - other.y, z - other.z };
	}
	Vector3 Vector3::operator-() const {
		// Calculate the resulting vector's dimensions
		return { -x, -y, -z };
	}
	Vector3 Vector3::operator*(const Vector3& other) const {
		// Calculate the resulting vector's dimensions
		return { x * other.x, y * other.y, z * other.z };
	}
	Vector3 Vector3::operator/(const Vector3& other) const {
		// Calculate the resulting vector's dimensions
		return { x / other.x, y / other.y, z / other.z };
	}

	Vector3& Vector3::operator+=(const Vector3& other) {
		// Increase each element
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}
	Vector3& Vector3::operator-=(const Vector3& other) {
		// Decrease each element
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}
	Vector3& Vector3::operator*=(const Vector3& other) {
		// Multiply each element
		x *= other.x;
		y *= other.y;
		z *= other.z;

		return *this;
	}
	Vector3& Vector3::operator*=(float other) {
		// Multiply each element
		x *= other;
		y *= other;
		z *= other;

		return *this;
	}
	Vector3& Vector3::operator/=(const Vector3& other) {
		// Divide each element
		x /= other.x;
		y /= other.y;
		z /= other.z;

		return *this;
	}
	Vector3& Vector3::operator/=(float other) {
		// Calculate the inverse of the given value
		float invOther = 1.0f / other;

		// Multiply each element with the inverse of the given value
		x *= invOther;
		y *= invOther;
		z *= invOther;

		return *this;
	}

	float Vector3::Magnitude() const {
		// Calculate the Vector3's magnitude
		return sqrt(x * x + y * y + z * z);
	}
	float Vector3::SqrMagnitude() const {
		// Calculate the Vector3's square magnitude
		return x * x + y * y + z * z;
	}
	float Vector3::InvMagnitude() const {
		// Calculate the Vector3's inverse magnitude
		return 1.0f / sqrt(x * x + y * y + z * z);
	}

	Vector3& Vector3::Normalize() {
		// Calculate the Vector3's inverse magnitude
		float invMag = InvMagnitude();

		// Multiply each element by the inverse magnitude
		x *= invMag;
		y *= invMag;
		z *= invMag;

		return *this;
	}
	Vector3 Vector3::Normalized() const {
		// Calculate the Vector3's inverse magnitude
		float invMag = InvMagnitude();

		// Calculate the resulting Vector3's elements
		return { x * invMag, y * invMag, z * invMag };
	}
	Vector3& Vector3::Flip() {
		// Set each element to its opposite
		x = -x;
		y = -y;
		z = -z;

		return *this;
	}
	Vector3 Vector3::Flipped() const {
		// Calculate the resulting Vector3's elements
		return { -x, -y, -z };
	}

	float Vector3::Dot(const Vector3& other) const {
		// Calculate the dot product
		return x * other.x + y * other.y + z * other.z;
	}
	float Vector3::Angle(const Vector3& other) const {
		// Calculate the dot product
		float dot = Dot(other);

		// Calculate the cosine of the angle using the dot product
		float angleCos = dot * InvMagnitude() * other.InvMagnitude();

		// Calculate the angle between the two Vector3s 
		return acosf(angleCos);
	}
    Vector3 Vector3::Cross(const Vector3& other) const {
        // Calculate the cross product
        return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
    }

	Vector3 operator*(const Vector3& vec, float val) {
		// Calculate the resulting vector's dimensions
		return { vec.x * val, vec.y * val, vec.z * val };
	}
	Vector3 operator*(float val, const Vector3& vec) {
		// Calculate the resulting vector's dimensions
		return { vec.x * val, vec.y * val, vec.z * val };
	}
	Vector3 operator/(const Vector3& vec, float val) {
		// Calculate the inverse of the given value
		float invVal = 1.0f / val;

		// Calculate the resulting vector's dimensions
		return { vec.x * invVal, vec.y * invVal, vec.z * invVal };
	}
	Vector3 operator/(float val, const Vector3& vec) {
		// Calculate the resulting vector's dimensions
		return { val / vec.x, val / vec.y, val / vec.z };
	}
}

// Hash function
template<>
struct std::hash<wfe::Vector3> {
	std::size_t operator()(const wfe::Vector3& vec) {
		// Use the boost library hash combine to mix all hashes
		std::hash<float> hasher;
		size_t res = 0;
		for(size_t i = 0; i != 3; ++i)
			res ^= hasher(vec.elements[i]) + 0x9e3779b9 + (res << 6) + (res >> 2);
		
		return res;
	}
};