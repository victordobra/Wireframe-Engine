#pragma once
#include <cstring>
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

namespace mge {
	class Matrix4x4 {
	public:
		Matrix4x4() = default;
		Matrix4x4(const Matrix4x4&) = default;
		Matrix4x4(Matrix4x4&&) noexcept = default;
		Matrix4x4(float mat[4][4]);

		float* operator[](size_t i) { return (float*)matrix + 4 * i; }
		const float* operator[](size_t i) const { return (const float*)matrix + 4 * i; }

		Matrix4x4 operator*(const Matrix4x4& other) const;
		Matrix4x4 operator*(Matrix4x4&& other) const;
		Vector3 operator*(const Vector3& other) const;
		Vector3 operator*(Vector3&& other) const;
		Vector4 operator*(const Vector4& other) const;
		Vector4 operator*(Vector4&& other) const;
		
		Matrix4x4& operator=(const Matrix4x4&) = default;
		Matrix4x4& operator=(Matrix4x4&&) noexcept = default;
		Matrix4x4& operator*=(const Matrix4x4& other) { *this = operator*(other); return *this; }
		Matrix4x4& operator*=(Matrix4x4&& other) noexcept { *this = operator*(other); return *this; }
		
		bool operator==(const Matrix4x4& other) const { return memcmp(other.matrix, matrix, sizeof(float) * 16); }
		bool operator==(Matrix4x4&& other) const { return memcmp(other.matrix, matrix, sizeof(float) * 16); }
		bool operator!=(const Matrix4x4& other) const { return !memcmp(other.matrix, matrix, sizeof(float) * 16); }
		bool operator!=(Matrix4x4&& other) const { return !memcmp(other.matrix, matrix, sizeof(float) * 16); }

		static Matrix4x4 Translation(Vector3 translation);
		static Matrix4x4 Rotation(Quaternion rotation);
		static Matrix4x4 Scaling(Vector3 scaling);

		~Matrix4x4() = default;
	private:
		float matrix[4][4]{ { 1, 0, 0, 0 },
							{ 0, 1, 0, 0 },
							{ 0, 0, 1, 0 },
							{ 0, 0, 0, 1 } };
	};
}