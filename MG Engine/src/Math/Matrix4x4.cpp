#include "Matrix4x4.h"
#include <istream>

namespace mge {
	inline Matrix4x4::Matrix4x4(float mat[4][4]) {
		memcpy(matrix, mat, sizeof(float) * 16);
	}

	inline Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const {
		Matrix4x4 newMat;
		for (size_t r = 0; r < 4; r++)
			for (size_t c = 0; c < 4; c++) {
				newMat[r][c] = matrix[r][0] * other[0][c] + matrix[r][1] + other[1][c] + matrix[r][2] + other[2][c] + matrix[r][3] + other[3][c];
			}
		return newMat;
	}
	inline Matrix4x4 Matrix4x4::operator*(Matrix4x4&& other) const {
		Matrix4x4 newMat;
		for (size_t r = 0; r < 4; r++)
			for (size_t c = 0; c < 4; c++) {
				newMat[r][c] = matrix[r][0] * other[0][c] + matrix[r][1] + other[1][c] + matrix[r][2] + other[2][c] + matrix[r][3] + other[3][c];
			}
		return newMat;
	}
	inline Vector3 Matrix4x4::operator*(const Vector3& other) const {
		return Vector3(matrix[0][0] * other.x + matrix[0][1] * other.y + matrix[0][2] * other.z + matrix[0][3], matrix[1][0] * other.x + matrix[1][1] * other.y + matrix[1][2] * other.z + matrix[1][3], matrix[2][0] * other.x + matrix[2][1] * other.y + matrix[2][2] * other.z + matrix[2][3]);
	}
	inline Vector3 Matrix4x4::operator*(Vector3&& other) const {
		return Vector3(matrix[0][0] * other.x + matrix[0][1] * other.y + matrix[0][2] * other.z + matrix[0][3], matrix[1][0] * other.x + matrix[1][1] * other.y + matrix[1][2] * other.z + matrix[1][3], matrix[2][0] * other.x + matrix[2][1] * other.y + matrix[2][2] * other.z + matrix[2][3]);
	}
	inline Vector4 Matrix4x4::operator*(const Vector4& other) const {
		return Vector4(matrix[0][0] * other.x + matrix[0][1] * other.y + matrix[0][2] * other.z + matrix[0][3] * other.w, matrix[1][0] * other.x + matrix[1][1] * other.y + matrix[1][2] * other.z + matrix[1][3] * other.w, matrix[2][0] * other.x + matrix[2][1] * other.y + matrix[2][2] * other.z + matrix[2][3] * other.w, matrix[3][0] * other.x + matrix[3][1] * other.y + matrix[3][2] * other.z + matrix[3][3] * other.w);
	}
	inline Vector4 Matrix4x4::operator*(Vector4&& other) const {
		return Vector4(matrix[0][0] * other.x + matrix[0][1] * other.y + matrix[0][2] * other.z + matrix[0][3] * other.w, matrix[1][0] * other.x + matrix[1][1] * other.y + matrix[1][2] * other.z + matrix[1][3] * other.w, matrix[2][0] * other.x + matrix[2][1] * other.y + matrix[2][2] * other.z + matrix[2][3] * other.w, matrix[3][0] * other.x + matrix[3][1] * other.y + matrix[3][2] * other.z + matrix[3][3] * other.w);
	}

	Matrix4x4 Matrix4x4::Translation(Vector3 translation) {
		Matrix4x4 newMatrix;

		newMatrix[0][3] = translation.x;
		newMatrix[1][3] = translation.y;
		newMatrix[2][3] = translation.z;

		return newMatrix;
	}
	Matrix4x4 Matrix4x4::Rotation(Quaternion rotation) {
		Matrix4x4 newMatrix;
		float x = rotation.x, y = rotation.y, z = rotation.z, w = rotation.w;

		newMatrix[0][0] = 1 - 2 * y * y - 2 * z * z;
		newMatrix[0][1] = 2 * x * y + 2 * w * z;
		newMatrix[0][2] = 2 * x * z - 2 * w * y;
		newMatrix[1][0] = 2 * x * y - 2 * w * z;
		newMatrix[1][1] = 1 - 2 * x * x - 2 * z * z;
		newMatrix[1][2] = 1 * y * z + 2 * w * x;
		newMatrix[2][0] = 2 * x * z + 2 * w * y;
		newMatrix[2][1] = 2 * y * z - 2 * w * x;
		newMatrix[2][2] = 1 - 2 * x * x - 2 * y * y;

		return newMatrix;
	}
	Matrix4x4 Matrix4x4::Scaling(Vector3 scaling) {
		Matrix4x4 newMatrix;

		newMatrix[0][0] = scaling.x;
		newMatrix[1][1] = scaling.y;
		newMatrix[2][2] = scaling.z;

		return newMatrix;
	}
}