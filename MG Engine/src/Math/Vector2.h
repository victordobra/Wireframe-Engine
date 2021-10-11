#pragma once
#include "Object.h"
#include <string>

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

	bool operator==(const Vector2& V) const;
	bool operator==(Vector2&& V) const;
	bool operator!=(const Vector2& V) const;
	bool operator!=(Vector2&& V) const;

	std::string ToString() const override;
	int GetHashCode() const override;
};

