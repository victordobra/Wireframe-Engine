#pragma once
#include "Object.h"
#include "Vector3.h"
#include <string>

class Quaternion : public Object {
public:
	float X, Y, Z, W;

	Quaternion();
	Quaternion(const Quaternion& Q);
	Quaternion(Quaternion&& Q) noexcept;
	Quaternion(float X, float Y, float Z, float W);

	void Normalize();
	Quaternion Normalized() const;
	float Magnitude() const;
	float SqrMagnitude() const;

	bool operator==(const Quaternion& Q) const;
	bool operator==(Quaternion&& Q) const;
	bool operator!=(const Quaternion& Q) const;
	bool operator!=(Quaternion&& Q) const;
	Quaternion operator*(const Quaternion& Q) const;
	Quaternion operator*(Quaternion&& Q) const;

	static Quaternion EulerAngles(float X, float Y, float Z);
	static Quaternion EulerAngles(Vector3 EulerAngles);
	static Quaternion AroundAxis(float Angle, Vector3 Axis);

	std::string ToString() const override;
	int GetHashCode() const override;
};

