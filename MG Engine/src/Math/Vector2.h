#pragma once
#include "Object.h"

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

	Vector2& operator=(const Vector2& V);
	Vector2& operator=(Vector2&& V) noexcept;

	bool operator==(const Vector2& V) const;
	bool operator==(Vector2&& V) const;
	bool operator!=(const Vector2& V) const;
	bool operator!=(Vector2&& V) const;

	explicit Vector2(const Vector3& V);
	explicit Vector2(Vector3&& V);
	explicit Vector2(const Vector4& V);
	explicit Vector2(Vector4&& V);

	static float Dot(const Vector2& X, const Vector2& Y);
	static float Dot(Vector2&& X, const Vector2& Y);
	static float Dot(const Vector2& X, Vector2&& Y);
	static float Dot(Vector2&& X, Vector2&& Y);

	std::string ToString() const override;
	int GetHashCode() const override;

	~Vector2() { }
};