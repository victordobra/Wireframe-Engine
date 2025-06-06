#pragma once

#include "Core/Types/Defines.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace wfe {
	/// @brief A class defining a 4x4 transform matrix.
	class Matrix4x4 {
	public:
		/// @brief An identity matrix.
		static const Matrix4x4 IDENTITY;

		union {
			/// @brief An array containing the matrix's data.
			float data[16];
			/// @brief The matrix containing the matrix's elements.
			float mat[4][4];
			/// @brief An array of all of the matrix's rows as Vector4s.
			Vector4 rows[4];

			struct {
				float m00, m01, m02, m03;
				float m10, m11, m12, m13;
				float m20, m21, m22, m23;
				float m30, m31, m32, m33;
			};
		};

		/// @brief Creates an identity matrix.
		Matrix4x4();
		/// @brief Copies the given matrix's data.
		/// @param other The matrix to copy.
		Matrix4x4(const Matrix4x4& other) = default;
		/// @brief Moves the given matrix's contents.
		/// @param other The matrix to move.
		Matrix4x4(Matrix4x4&& other) noexcept = default;
		/// @brief Creates a matrix with the given elements.
		/// @param m00 The value at the first row and first column.
		/// @param m01 The value at the first row and second column.
		/// @param m02 The value at the first row and third column.
		/// @param m03 The value at the first row and fourth column.
		/// @param m10 The value at the second row and first column.
		/// @param m11 The value at the second row and second column.
		/// @param m12 The value at the second row and third column.
		/// @param m13 The value at the second row and fourth column.
		/// @param m20 The value at the third row and first column.
		/// @param m21 The value at the third row and second column.
		/// @param m22 The value at the third row and third column.
		/// @param m23 The value at the third row and fourth column.
		/// @param m30 The value at the fourth row and first column.
		/// @param m31 The value at the fourth row and second column.
		/// @param m32 The value at the fourth row and third column.
		/// @param m33 The value at the fourth row and fourth column.
		Matrix4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);
		/// @brief Creates a matrix with the given elements, filling the remaining space with identity values.
		/// @param m00 The value at the first row and first column.
		/// @param m01 The value at the first row and second column.
		/// @param m02 The value at the first row and third column.
		/// @param m10 The value at the second row and first column.
		/// @param m11 The value at the second row and second column.
		/// @param m12 The value at the second row and third column.
		/// @param m20 The value at the third row and first column.
		/// @param m21 The value at the third row and second column.
		/// @param m22 The value at the third row and third column.
		Matrix4x4(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22);
		/// @brief Creates a matrix with the given Vector4s set to corresponding rows.
		/// @param row1 The matrix's first row.
		/// @param row2 The matrix's second row.
		/// @param row3 The matrix's third row.
		/// @param row4 The matrix's fourth row.
		Matrix4x4(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4);
		/// @brief Creates a matrix with the given rows and will all empty spaces set to identity values.
		/// @param row1 The matrix's first row.
		/// @param row2 The matrix's second row.
		/// @param row3 The matrix's third row.
		Matrix4x4(const Vector3& row1, const Vector3& row2, const Vector3& row3);
		/// @brief Creates a matrix with the given data.
		/// @param data The matrix's data.
		Matrix4x4(const float data[16]);
		/// @brief Creates a matrix with the given values.
		/// @param mat The matrix's values.
		Matrix4x4(const float mat[4][4]);

		/// @brief Copies the given matrix's contents into this matrix.
		/// @param other The matrix to copy.
		/// @return A reference to this matrix.
		Matrix4x4& operator=(const Matrix4x4& other) = default;
		/// @brief Moves the given matrix's contents into this matrix.
		/// @param other The matrix to move.
		/// @return A reference to this matrix.
		Matrix4x4& operator=(Matrix4x4&& other) noexcept = default;

		/// @brief Returns the row with the given index.
		/// @param index The index to return.
		/// @return A pointer to the requested row.
		float* operator[](size_t index);
		/// @brief Returns the row with the given index.
		/// @param index The index to return.
		/// @return A const pointer to the requested row.
		const float* operator[](size_t index) const;

		/// @brief Compares the two matrices.
		/// @param other THe matrix to compare with.
		/// @return True if the two matrices are equal, otherwise false.
		bool operator==(const Matrix4x4& other) const;
		/// @brief Compares the two matrices.
		/// @param other THe matrix to compare with.
		/// @return True if the two matrices are different, otherwise false.
		bool operator!=(const Matrix4x4& other) const;

		/// @brief Multiplies the two matrices.
		/// @param other The matrix to multiply with.
		/// @return The product of the two matrices.
		Matrix4x4 operator*(const Matrix4x4& other) const;
		/// @brief Multiplies this matrix with the given matrix.
		/// @param other The matrix to multiply.
		/// @return A reference to this matrix.
		Matrix4x4& operator*=(const Matrix4x4& other);

		/// @brief Inverts this matrix.
		/// @return A reference to this matrix.
		Matrix4x4& Invert();
		/// @brief Calculates the inverse of this matrix.
		/// @return The inverse of this matrix.
		Matrix4x4 Inverse() const;
		/// @brief Transposes this matrix.
		/// @return A reference to this matrix.
		Matrix4x4& Transpose();
		/// @brief Calculates the transposed version of this matrix.
		/// @return The resulting matrix.
		Matrix4x4 Transposed() const;

		/// @brief Calculates the matrix's forward vector.
		/// @return The matrix's forward vector.
		Vector3 Forward() const;
		/// @brief Calculates the matrix's backward vector.
		/// @return The matrix's backward vector.
		Vector3 Backward() const;
		/// @brief Calculates the matrix's up vector.
		/// @return The matrix's up vector.
		Vector3 Up() const;
		/// @brief Calculates the matrix's down vector.
		/// @return The matrix's down vector.
		Vector3 Down() const;
		/// @brief Calculates the matrix's left vector.
		/// @return The matrix's left vector.
		Vector3 Left() const;
		/// @brief Calculates the matrix's right vector.
		/// @return The matrix's right vector.
		Vector3 Right() const;

		/// @brief Destroys the matrix.
		~Matrix4x4() = default;

		/// @brief Generates a perspective projection matrix.
		/// @param fov The vertical field of view, in radians.
		/// @param aspectRatio The aspect ratio.
		/// @param nearPlane The distance from the origin point to the near clipping plane.
		/// @param farPlane The distance from the origin point to the far clipping plane.
		/// @return The resulting matrix.
		static Matrix4x4 PerspectiveProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
		/// @brief Generates a ortographic projection matrix.
		/// @param left The left size of the view frustum.
		/// @param right The right size of the view frustum.
		/// @param bottom The bottom size of the view frustum.
		/// @param top The top size of the view frustum.
		/// @param nearPlane The distance from the origin point to the near clipping plane.
		/// @param farPlane The distance from the origin point to the far clipping plane.
		/// @return The resulting matrix.
		static Matrix4x4 OrtographicProjection(float left, float right, float bottom, float top, float nearPlane, float farPlane);
		/// @brief Generates a look-at matrix, or a matrix looking at the target position from the perspective of the given position.
		/// @param position The position of the matrix.
		/// @param target The position for the matrix to "look at".
		/// @param up The up vector. Must be normalized.
		/// @return The resulting matrix.
		static Matrix4x4 LookAt(Vector3 position, Vector3 target, Vector3 up);
		/// @brief Generates a transform matrix.
		/// @param position The position of the matix.
		/// @return The resulting matrix.
		static Matrix4x4 Translation(Vector3 position);
		/// @brief Generates a matrix for a rotation around the X axis.
		/// @param angle The angle of rotation in radians.
		/// @return The resulting matrix.
		static Matrix4x4 EulerX(float angle);
		/// @brief Generates a matrix for a rotation around the Y axis.
		/// @param angle The angle of rotation in radians.
		/// @return The resulting matrix.
		static Matrix4x4 EulerY(float angle);
		/// @brief Generates a matrix for a rotation around the Z axis.
		/// @param angle The angle of rotation in radians.
		/// @return The resulting matrix.
		static Matrix4x4 EulerZ(float angle);
		/// @brief Generates a matrix representing a rotation.
		/// @param rotation A quaternion representing a 3D rotation.
		/// @return The resulting matrix.
		static Matrix4x4 Rotation(Quaternion rotation);
		/// @brief Generates a scaling matrix.
		/// @param scale The scale of the matrix.
		/// @return The resulting matrix.
		static Matrix4x4 Scaling(Vector3 scale);
		/// @brief Generates a transform matrix.
		/// @param position The position of the transform.
		/// @param rotation The rotation of the transform.
		/// @param scale THe scale of the transform.
		/// @return The resulting matrix.
		static Matrix4x4 Transform(Vector3 position, Quaternion rotation, Vector3 scale);
	};

	/// @brief Multiplies the given vector with the given matrix.
	/// @param mat The matrix to multiply with.
	/// @param vec The vector to multiply.
	/// @return The resulting vector.
	Vector2 operator*(const Matrix4x4& mat, const Vector2& vec);
	/// @brief Multiplies the given vector with the given matrix.
	/// @param vec The vector to multiply.
	/// @param mat The matrix to multiply with.
	/// @return The resulting vector.
	Vector2 operator*(const Vector2& vec, const Matrix4x4& mat);
	/// @brief Multiplies the given vector with the given matrix.
	/// @param mat The matrix to multiply with.
	/// @param vec The vector to multiply.
	/// @return The resulting vector.
	Vector3 operator*(const Matrix4x4& mat, const Vector3& vec);
	/// @brief Multiplies the given vector with the given matrix.
	/// @param vec The vector to multiply.
	/// @param mat The matrix to multiply with.
	/// @return The resulting vector.
	Vector3 operator*(const Vector3& vec, const Matrix4x4& mat);
	/// @brief Multiplies the given vector with the given matrix.
	/// @param mat The matrix to multiply with.
	/// @param vec The vector to multiply.
	/// @return The resulting vector.
	Vector4 operator*(const Matrix4x4& mat, const Vector4& vec);
	/// @brief Multiplies the given vector with the given matrix.
	/// @param vec The vector to multiply.
	/// @param mat The matrix to multiply with.
	/// @return The resulting vector.
	Vector4 operator*(const Vector4& vec, const Matrix4x4& mat);
}