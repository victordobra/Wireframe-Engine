#include "Matrix4x4.h"
#include <istream>

Matrix4x4::Matrix4x4() {
	operator()(0, 0) = 1; operator()(1, 0) = 0; operator()(2, 0) = 0; operator()(3, 0) = 0;
	operator()(0, 1) = 0; operator()(1, 1) = 1; operator()(2, 1) = 0; operator()(3, 1) = 0;
	operator()(0, 2) = 0; operator()(1, 2) = 0; operator()(2, 2) = 1; operator()(3, 2) = 0;
	operator()(0, 3) = 0; operator()(1, 3) = 0; operator()(2, 3) = 0; operator()(3, 3) = 1;
}
Matrix4x4::Matrix4x4(float M[4][4]) {
	memcpy(Matrix, M, sizeof(float) * 16);
}

float& Matrix4x4::operator()(int X, int Y) {
	return Matrix[X][Y];
}
float Matrix4x4::operator()(int X, int Y) const {
	return Matrix[X][Y];
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& M) const {
	Matrix4x4 NewM;
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			NewM(x, y) = operator()(0, y) * M(x, 0) + operator()(1, y) * M(x, 1) + operator()(2, y) * M(x, 2) + operator()(3, y) * M(x, 3);
		}
	return NewM;
}
Matrix4x4 Matrix4x4::operator*(Matrix4x4&& M) const {
	Matrix4x4 NewM;
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			NewM(x, y) = operator()(0, y) * M(x, 0) + operator()(1, y) * M(x, 1) + operator()(2, y) * M(x, 2) + operator()(3, y) * M(x, 3);
		}
	return NewM;
}
Vector3 Matrix4x4::operator*(const Vector3& V) const {
	return Vector3(operator()(0, 0) * V.X + operator()(0, 1) * V.Y + operator()(0, 2) * V.Z + operator()(0, 3), operator()(1, 0) * V.X + operator()(1, 1) * V.Y + operator()(1, 2) * V.Z + operator()(1, 3), operator()(2, 0) * V.X + operator()(2, 1) * V.Y + operator()(2, 2) * V.Z + operator()(2, 3));
}
Vector3 Matrix4x4::operator*(Vector3&& V) const {
	return Vector3(operator()(0, 0) * V.X + operator()(0, 1) * V.Y + operator()(0, 2) * V.Z + operator()(0, 3), operator()(1, 0) * V.X + operator()(1, 1) * V.Y + operator()(1, 2) * V.Z + operator()(1, 3), operator()(2, 0) * V.X + operator()(2, 1) * V.Y + operator()(2, 2) * V.Z + operator()(2, 3));
}
Vector4 Matrix4x4::operator*(const Vector4& V) const {
	return Vector4(operator()(0, 0) * V.X + operator()(0, 1) * V.Y + operator()(0, 2) * V.Z + operator()(0, 3) * V.W, operator()(1, 0) * V.X + operator()(1, 1) * V.Y + operator()(1, 2) * V.Z + operator()(1, 3) * V.W, operator()(2, 0) * V.X + operator()(2, 1) * V.Y + operator()(2, 2) * V.Z + operator()(2, 3) * V.W, operator()(3, 0) * V.X + operator()(3, 1) * V.Y + operator()(3, 2) * V.Z + operator()(3, 3) * V.W);
}
Vector4 Matrix4x4::operator*(Vector4&& V) const {
	return Vector4(operator()(0, 0) * V.X + operator()(0, 1) * V.Y + operator()(0, 2) * V.Z + operator()(0, 3) * V.W, operator()(1, 0) * V.X + operator()(1, 1) * V.Y + operator()(1, 2) * V.Z + operator()(1, 3) * V.W, operator()(2, 0) * V.X + operator()(2, 1) * V.Y + operator()(2, 2) * V.Z + operator()(2, 3) * V.W, operator()(3, 0) * V.X + operator()(3, 1) * V.Y + operator()(3, 2) * V.Z + operator()(3, 3) * V.W);
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& M) {
	*this = operator*(M);

	return *this;
}
Matrix4x4& Matrix4x4::operator*=(Matrix4x4&& M) noexcept {
	*this = operator*(M);

	return *this;
}

bool Matrix4x4::operator==(const Matrix4x4& M) const {
	return memcmp(M.Matrix, Matrix, sizeof(float) * 16);
}
bool Matrix4x4::operator==(Matrix4x4&& M) const {
	return memcmp(M.Matrix, Matrix, sizeof(float) * 16);
}
bool Matrix4x4::operator!=(const Matrix4x4& M) const {
	return !memcmp(M.Matrix, Matrix, sizeof(float) * 16);
}
bool Matrix4x4::operator!=(Matrix4x4&& M) const {
	return !memcmp(M.Matrix, Matrix, sizeof(float) * 16);
}

Matrix4x4 Matrix4x4::Translation(const Vector3& Translation) {
	Matrix4x4 NewMatrix;

	NewMatrix(0, 3) = Translation.X;
	NewMatrix(1, 3) = Translation.Y;
	NewMatrix(2, 3) = Translation.Z;

	return NewMatrix;
}
Matrix4x4 Matrix4x4::Translation(Vector3&& Translation) {
	Matrix4x4 NewMatrix;

	NewMatrix(0, 3) = Translation.X;
	NewMatrix(1, 3) = Translation.Y;
	NewMatrix(2, 3) = Translation.Z;

	return NewMatrix;
}
Matrix4x4 Matrix4x4::Rotation(const Quaternion& Rotation) {
	Matrix4x4 NewMatrix;
	float X = Rotation.X, Y = Rotation.Y, Z = Rotation.Z, W = Rotation.W;

	NewMatrix(0, 0) = 1 - 2 * Y * Y - 2 * Z * Z;
	NewMatrix(0, 1) = 2 * X * Y + 2 * W * Z;
	NewMatrix(0, 2) = 2 * X * Z - 2 * W * Y;
	NewMatrix(1, 0) = 2 * X * Y - 2 * W * Z;
	NewMatrix(1, 1) = 1 - 2 * X * X - 2 * Z * Z;
	NewMatrix(1, 2) = 1 * Y * Z + 2 * W * X;
	NewMatrix(2, 0) = 2 * X * Z + 2 * W * Y;
	NewMatrix(2, 1) = 2 * Y * Z - 2 * W * X;
	NewMatrix(2, 2) = 1 - 2 * X * X - 2 * Y * Y;

	return NewMatrix;
}
Matrix4x4 Matrix4x4::Rotation(Quaternion&& Rotation) {
	Matrix4x4 NewMatrix;
	float X = Rotation.X, Y = Rotation.Y, Z = Rotation.Z, W = Rotation.W;

	NewMatrix(0, 0) = 1 - 2 * Y * Y - 2 * Z * Z;
	NewMatrix(0, 1) = 2 * X * Y + 2 * W * Z;
	NewMatrix(0, 2) = 2 * X * Z - 2 * W * Y;
	NewMatrix(1, 0) = 2 * X * Y - 2 * W * Z;
	NewMatrix(1, 1) = 1 - 2 * X * X - 2 * Z * Z;
	NewMatrix(1, 2) = 1 * Y * Z + 2 * W * X;
	NewMatrix(2, 0) = 2 * X * Z + 2 * W * Y;
	NewMatrix(2, 1) = 2 * Y * Z - 2 * W * X;
	NewMatrix(2, 2) = 1 - 2 * X * X - 2 * Y * Y;

	return NewMatrix;
}
Matrix4x4 Matrix4x4::Scaling(const Vector3& Scaling) {
	Matrix4x4 NewMatrix;

	NewMatrix(0, 0) = Scaling.X;
	NewMatrix(1, 1) = Scaling.Y;
	NewMatrix(2, 2) = Scaling.Z;

	return NewMatrix;
}
Matrix4x4 Matrix4x4::Scaling(Vector3&& Scaling) {
	Matrix4x4 NewMatrix;

	NewMatrix(0, 0) = Scaling.X;
	NewMatrix(1, 1) = Scaling.Y;
	NewMatrix(2, 2) = Scaling.Z;

	return NewMatrix;
}

std::string Matrix4x4::ToString() const {
	std::string String = "Matrix4x4(";
	for (int y = 0; y < 4; y++) {
		String += "[";
		for (int x = 0; x < 4; x++) {
			String += std::to_string(operator()(x, y));
			if (x != 3)
				String += ", ";
		}
		String += "]";
		if (y != 3)
			String += ", ";
	}
	String += ")";
	return String;
}
size_t Matrix4x4::GetHashCode() const {
	return typeid(Matrix4x4).hash_code();
}