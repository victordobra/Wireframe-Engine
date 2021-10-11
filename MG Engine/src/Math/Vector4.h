#pragma once
#include "Object.h"
#include <string>

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

	bool operator==(const Vector4& V) const;
	bool operator==(Vector4&& V) const;
	bool operator!=(const Vector4& V) const;
	bool operator!=(Vector4&& V) const;

	std::string ToString() const override;
	int GetHashCode() const override;
};

