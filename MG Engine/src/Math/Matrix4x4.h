#pragma once
#include "Object.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

class Matrix4x4 : public Object {
public:
	float Matrix[4][4];

	Matrix4x4();
	Matrix4x4(const Matrix4x4& M);
	Matrix4x4(Matrix4x4&& M) noexcept;
	Matrix4x4(float* M);

	float* operator[](int X) const;

	Matrix4x4 operator*(const Matrix4x4& M) const;
	Matrix4x4 operator*(Matrix4x4&& M) const;
	Vector3 operator*(const Vector3& V) const;
	Vector3 operator*(Vector3&& V) const;
	Vector4 operator*(const Vector4& V) const;
	Vector4 operator*(Vector4&& V) const;

	Matrix4x4& operator=(const Matrix4x4& M);
	Matrix4x4& operator=(Matrix4x4&& M) noexcept;

	bool operator==(const Matrix4x4& M) const;
	bool operator==(Matrix4x4&& M) const;
	bool operator!=(const Matrix4x4& M) const;
	bool operator!=(Matrix4x4&& M) const;

	static Matrix4x4 Translation(Vector3 Translation);
	static Matrix4x4 Rotation(Quaternion Rotation);
	static Matrix4x4 Scaling(Vector3 Scaling);

	std::string ToString() const override;
	int GetHashCode() const override;

	~Matrix4x4() { }
};

