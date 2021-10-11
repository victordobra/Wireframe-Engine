#pragma once
#include "Object.h"
#include <string>

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
	
	bool operator==(const Vector3& V) const;
	bool operator==(Vector3&& V) const;
	bool operator!=(const Vector3& V) const;
	bool operator!=(Vector3&& V) const;

	std::string ToString() const override;
	int GetHashCode() const override;
};

