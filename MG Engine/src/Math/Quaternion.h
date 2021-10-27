#pragma once
#include "Object.h"
#include <string>

class Vector3;
class Vector4;

class Quaternion : public Object {
public:
	float X, Y, Z, W;

	Quaternion();
	Quaternion(const Quaternion& Q);
	Quaternion(Quaternion&& Q) noexcept;
	Quaternion(float X, float Y, float Z, float W);
	Quaternion(Vector3 V, float W);

	void Normalize();
	Quaternion Normalized() const;
	void Invert();
	Quaternion Inverted() const;
	float Magnitude() const;
	float SqrMagnitude() const;
	
	Quaternion operator*(const Quaternion& Q) const;
	Quaternion operator*(Quaternion&& Q) const;
	Quaternion operator/(const Quaternion& Q) const;
	Quaternion operator/(Quaternion&& Q) const;

	Quaternion& operator=(const Quaternion& Q);
	Quaternion& operator=(Quaternion&& Q) noexcept;

	bool operator==(const Quaternion& Q) const;
	bool operator==(Quaternion&& Q) const;
	bool operator!=(const Quaternion& Q) const;
	bool operator!=(Quaternion&& Q) const;

	explicit Quaternion(const Vector3& V);
	explicit Quaternion(Vector3&& V);
	explicit Quaternion(const Vector4& V);
	explicit Quaternion(Vector4&& V);

	static Quaternion EulerAngles(float X, float Y, float Z);
	static Quaternion EulerAngles(Vector3 EulerAngles);
	static Quaternion AroundAxis(float Angle, Vector3 Axis);

	std::string ToString() const override;
	int GetHashCode() const override;

	~Quaternion() { }
};

