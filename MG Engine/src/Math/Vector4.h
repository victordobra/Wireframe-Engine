#pragma once
#include "Object.h"

class Vector2;
class Vector3;
class Quaternion;

class Vector4 : public Object {
public:
	float X, Y, Z, W;

	Vector4();
	Vector4(const Vector4& V);
	Vector4(Vector4&& V) noexcept;
	Vector4(float X, float Y, float Z, float W);

	void Normalize();
	Vector4 Normalized() const;
	float Magnitude() const;
	float SqrMagnitude() const;
	void ClampMagnitude(float Min, float Max);

	Vector4 operator+(const Vector4& V) const;
	Vector4 operator+(Vector4&& V) const;
	Vector4 operator-(const Vector4& V) const;
	Vector4 operator-(Vector4&& V) const;
	Vector4 operator*(float N) const;
	Vector4 operator/(float N) const;

	Vector4& operator=(const Vector4& V);
	Vector4& operator=(Vector4&& V) noexcept;

	bool operator==(const Vector4& V) const;
	bool operator==(Vector4&& V) const;
	bool operator!=(const Vector4& V) const;
	bool operator!=(Vector4&& V) const;

	Vector4(const Vector2& V);
	Vector4(Vector2&& V);
	Vector4(const Vector3& V);
	Vector4(Vector3&& V);
	explicit Vector4(const Quaternion& Q);
	explicit Vector4(Quaternion&& Q);

	static float Dot(const Vector4& X, const Vector4& Y);
	static float Dot(Vector4&& X, const Vector4& Y);
	static float Dot(const Vector4& X, Vector4&& Y);
	static float Dot(Vector4&& X, Vector4&& Y);

	std::string ToString() const override;
	int GetHashCode() const override;

	~Vector4() { }
};