#pragma once
#include "Object.h"
#include "Vector3.h"
#include "Vector4.h"

class Vector3;
class Vector4;

class Vector2 : public Object {
public:
	float X, Y;

	Vector2();
	Vector2(const Vector2& V);
	Vector2(Vector2&& V) noexcept;
	Vector2(float X, float Y);

	void Normalize();
	Vector2 Normalized() const;
	float Magnitude() const;
	float SqrMagnitude() const;
	void ClampMagnitude(float Min, float Max);

	Vector2 operator+(const Vector2& V) const;
	Vector2 operator+(Vector2&& V) const;
	Vector2 operator-(const Vector2& V) const;
	Vector2 operator-(Vector2&& V) const;
	Vector2 operator*(float N) const;
	Vector2 operator/(float N) const;
	bool operator==(const Vector2& V) const;
	bool operator==(Vector2&& V) const;
	bool operator!=(const Vector2& V) const;
	bool operator!=(Vector2&& V) const;

	explicit Vector2(Vector3 V);
	explicit Vector2(Vector4 V);

	static Vector2 Dot(Vector2 X, Vector2 Y);

	std::string ToString() const override;
	int GetHashCode() const override;
};