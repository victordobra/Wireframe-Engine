#pragma once
#include "Object.h"
#include "Vector2.h"
#include "Vector4.h"

class Vector2;
class Vector4;

class Vector3 : public Object {
public:
	float X, Y, Z;

	Vector3();
	Vector3(const Vector3& V);
	Vector3(Vector3&& V) noexcept;
	Vector3(float X, float Y);
	Vector3(float X, float Y, float Z);

	void Normalize();
	Vector3 Normalized() const;
	float Magnitude() const;
	float SqrMagnitude() const;
	void ClampMagnitude(float Min, float Max);

	Vector3 operator+(const Vector3& V) const;
	Vector3 operator+(Vector3&& V) const;
	Vector3 operator-(const Vector3& V) const;
	Vector3 operator-(Vector3&& V) const;
	Vector3 operator*(float N) const;
	Vector3 operator/(float N) const;
	bool operator==(const Vector3& V) const;
	bool operator==(Vector3&& V) const;
	bool operator!=(const Vector3& V) const;
	bool operator!=(Vector3&& V) const;

	Vector3(Vector2 V);
	explicit Vector3(Vector4 V);

	static Vector3 Dot(Vector3 X, Vector3 Y);

	std::string ToString() const override;
	int GetHashCode() const override;
};