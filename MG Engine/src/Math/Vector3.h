#pragma once
#include "Object.h"

class Vector2;
class Vector4;
class Quaternion;

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

	Vector3& operator=(const Vector3& V);
	Vector3& operator=(Vector3&& V) noexcept;

	bool operator==(const Vector3& V) const;
	bool operator==(Vector3&& V) const;
	bool operator!=(const Vector3& V) const;
	bool operator!=(Vector3&& V) const;

	Vector3(const Vector2& V);
	Vector3(Vector2&& V);
	explicit Vector3(const Vector4& V);
	explicit Vector3(Vector4&& V);
	explicit Vector3(const Quaternion& Q);
	explicit Vector3(Quaternion&& Q);

	static float Dot(const Vector3& X, const Vector3& Y);
	static float Dot(Vector3&& X, const Vector3& Y);
	static float Dot(const Vector3& X, Vector3&& Y);
	static float Dot(Vector3&& X, Vector3&& Y);

	std::string ToString() const override;
	int GetHashCode() const override;

	~Vector3() { }
};