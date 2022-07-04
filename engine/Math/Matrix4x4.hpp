#pragma once

#include "Core.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"
#include <string.h>
#include <math.h>

namespace wfe {
	struct Matrix4x4 {
		/// @brief Returns the identity matrix.
		static Matrix4x4 Identity() { return Matrix4x4(); }

		union {
			float32_t data[16]{ 1.f, 0.f, 0.f, 0.f,
			    	            0.f, 1.f, 0.f, 0.f,
			    	            0.f, 0.f, 1.f, 0.f,
			    	            0.f, 0.f, 0.f, 1.f };
			float32_t mat[4][4];

			struct {
				float32_t m00, m01, m02, m03;
				float32_t m10, m11, m12, m13;
				float32_t m20, m21, m22, m23;
				float32_t m30, m31, m32, m33;
			};
		};

		Matrix4x4() = default;
		Matrix4x4(const Matrix4x4&) = default;
		Matrix4x4(Matrix4x4&&) noexcept = default;

		float32_t& operator()(uint8_t c, uint8_t r)       { return mat[r][c]; }
		float32_t  operator()(uint8_t c, uint8_t r) const { return mat[r][c]; }

		Matrix4x4& operator=(const Matrix4x4&) = default;
		Matrix4x4& operator=(Matrix4x4&&) noexcept = default;

		bool8_t operator==(const Matrix4x4& other) const { 
			for (uint8_t i = 0; i < 16; i++)
				if (data[i] != other.data[i])
					return false; 

			return true; 
		}
		bool8_t operator==(Matrix4x4&& other)      const { 
			for (uint8_t i = 0; i < 16; i++)
				if (data[i] != other.data[i])
					return false;

			return true;
		}
		bool8_t operator!=(const Matrix4x4& other) const { 
			for (uint8_t i = 0; i < 16; i++)
				if (data[i] != other.data[i])
					return false;

			return true;
		}
		bool8_t operator!=(Matrix4x4&& other)      const { 
			for (uint8_t i = 0; i < 16; i++)
				if (data[i] != other.data[i])
					return false;

			return true;
		}

		Matrix4x4 operator*(const Matrix4x4& other) const {
			Matrix4x4 outMatrix;

			const float32_t* m1Ptr = data;
			const float32_t* m2Ptr = other.data;
			float32_t* outPtr = outMatrix.data;

			for (uint8_t i = 0; i < 4; ++i) {
				for (uint8_t j = 0; j < 4; j++) {
					*outPtr = m1Ptr[0] * m2Ptr[j] + m1Ptr[1] * m2Ptr[4 + j] + m1Ptr[2] * m2Ptr[8 + j] + m1Ptr[3] * m2Ptr[12 + j];
					++outPtr;
				}
				m1Ptr += 4;
			}

			return outMatrix;
		}
		Matrix4x4 operator*(Matrix4x4&& other) const {
			Matrix4x4 outMatrix;

			const float32_t* m1Ptr = data;
			const float32_t* m2Ptr = other.data;
			float32_t* outPtr = outMatrix.data;

			for (uint8_t i = 0; i < 4; ++i) {
				for (uint8_t j = 0; j < 4; j++) {
					*outPtr = m1Ptr[0] * m2Ptr[j] + m1Ptr[1] * m2Ptr[4 + j] + m1Ptr[2] * m2Ptr[8 + j] + m1Ptr[3] * m2Ptr[12 + j];
					++outPtr;
				}
				m1Ptr += 4;
			}

			return outMatrix;
		}

		Vector2 operator*(const Vector2& other) const {
			return { other.x * m00 + other.y * m01 + m02 + m03, other.x * m10 + other.y * m11 + m12 + m13 };
		}
		Vector2 operator*(Vector2&& other) const {
			return { other.x * m00 + other.y * m01 + m02 + m03, other.x * m10 + other.y * m11 + m12 + m13 };
		}
		Vector3 operator*(const Vector3& other) const {
			return { other.x * m00 + other.y * m01 + other.z * m02 + m03, other.x * m10 + other.y * m11 + other.z * m12 + m13, other.x * m20 + other.y * m21 + other.z * m22 + m23 };
		}
		Vector3 operator*(Vector3&& other) const {
			return { other.x * m00 + other.y * m01 + other.z * m02 + m03, other.x * m10 + other.y * m11 + other.z * m12 + m13, other.x * m20 + other.y * m21 + other.z * m22 + m23 };
		}
		Vector4 operator*(const Vector4& other) const {
			return { other.x * m00 + other.y * m01 + other.z * m02 + other.w * m03, other.x * m10 + other.y * m11 + other.z * m12 + other.w * m13, other.x * m20 + other.y * m21 + other.z * m22 + other.w * m23, other.x * m30 + other.y * m31 + other.z * m32 + other.w * m33 };
		}
		Vector4 operator*(Vector4&& other) const {
			return { other.x * m00 + other.y * m01 + other.z * m02 + other.w * m03, other.x * m10 + other.y * m11 + other.z * m12 + other.w * m13, other.x * m20 + other.y * m21 + other.z * m22 + other.w * m23, other.x * m30 + other.y * m31 + other.z * m32 + other.w * m33 };
		}

		Matrix4x4& operator*=(const Matrix4x4& other) {
			float32_t m1[16];
			memcpy(m1, data, sizeof(float32_t) * 16);

			const float32_t* m1Ptr = m1;
			const float32_t* m2Ptr = other.data;
			float32_t* outPtr = data;

			for (uint8_t i = 0; i < 4; ++i) {
				for (uint8_t j = 0; j < 4; j++) {
					*outPtr = m1Ptr[0] * m2Ptr[j] + m1Ptr[1] * m2Ptr[4 + j] + m1Ptr[2] * m2Ptr[8 + j] + m1Ptr[3] * m2Ptr[12 + j];
					++outPtr;
				}
				m1Ptr += 4;
			}

			return *this;
		}
		Matrix4x4& operator*=(Matrix4x4&& other) noexcept {
			float32_t m1[16];
			memcpy(m1, data, sizeof(float32_t) * 16);

			const float32_t* m1Ptr = m1;
			const float32_t* m2Ptr = other.data;
			float32_t* outPtr = data;

			for (uint8_t i = 0; i < 4; ++i) {
				for (uint8_t j = 0; j < 4; j++) {
					*outPtr = m1Ptr[0] * m2Ptr[j] + m1Ptr[1] * m2Ptr[4 + j] + m1Ptr[2] * m2Ptr[8 + j] + m1Ptr[3] * m2Ptr[12 + j];
					++outPtr;
				}
				m1Ptr += 4;
			}

			return *this;
		}

		/// @brief Inverts the matrix.
		Matrix4x4& Invert() {
			//Big no touchie sign hire unless you know what this means, which I for sure don't
			float32_t m[16];
			memcpy(m, data, sizeof(float32_t) * 16);

			float32_t t0 = m[10] * m[15];
			float32_t t1 = m[14] * m[11];
			float32_t t2 = m[6] * m[15];
			float32_t t3 = m[14] * m[7];
			float32_t t4 = m[6] * m[11];
			float32_t t5 = m[10] * m[7];
			float32_t t6 = m[2] * m[15];
			float32_t t7 = m[14] * m[3];
			float32_t t8 = m[2] * m[11];
			float32_t t9 = m[10] * m[3];
			float32_t t10 = m[2] * m[7];
			float32_t t11 = m[6] * m[3];
			float32_t t12 = m[8] * m[13];
			float32_t t13 = m[12] * m[9];
			float32_t t14 = m[4] * m[13];
			float32_t t15 = m[12] * m[5];
			float32_t t16 = m[4] * m[9];
			float32_t t17 = m[8] * m[5];
			float32_t t18 = m[0] * m[13];
			float32_t t19 = m[12] * m[1];
			float32_t t20 = m[0] * m[9];
			float32_t t21 = m[8] * m[1];
			float32_t t22 = m[0] * m[5];
			float32_t t23 = m[4] * m[1];

			float32_t* o = data;

			o[0] = (t0 * m[5] + t3 * m[9] + t4 * m[13]) - (t1 * m[5] + t2 * m[9] + t5 * m[13]);
			o[1] = (t1 * m[1] + t6 * m[9] + t9 * m[13]) - (t0 * m[1] + t7 * m[9] + t8 * m[13]);
			o[2] = (t2 * m[1] + t7 * m[5] + t10 * m[13]) - (t3 * m[1] + t6 * m[5] + t11 * m[13]);
			o[3] = (t5 * m[1] + t8 * m[5] + t11 * m[9]) - (t4 * m[1] + t9 * m[5] + t10 * m[9]);

			float32_t d = 1.f / (m[0] * o[0] + m[4] * o[1] + m[8] * o[2] + m[12] * o[3]);

			o[0] = d * o[0];
			o[1] = d * o[1];
			o[2] = d * o[2];
			o[3] = d * o[3];
			o[4] = d * ((t1 * m[4] + t2 * m[8] + t5 * m[12]) - (t0 * m[4] + t3 * m[8] + t4 * m[12]));
			o[5] = d * ((t0 * m[0] + t7 * m[8] + t8 * m[12]) - (t1 * m[0] + t6 * m[8] + t9 * m[12]));
			o[6] = d * ((t3 * m[0] + t6 * m[4] + t11 * m[12]) - (t2 * m[0] + t7 * m[4] + t10 * m[12]));
			o[7] = d * ((t4 * m[0] + t9 * m[4] + t10 * m[8]) - (t5 * m[0] + t8 * m[4] + t11 * m[8]));
			o[8] = d * ((t12 * m[7] + t15 * m[11] + t16 * m[15]) - (t13 * m[7] + t14 * m[11] + t17 * m[15]));
			o[9] = d * ((t13 * m[3] + t18 * m[11] + t21 * m[15]) - (t12 * m[3] + t19 * m[11] + t20 * m[15]));
			o[10] = d * ((t14 * m[3] + t19 * m[7] + t22 * m[15]) - (t15 * m[3] + t18 * m[7] + t23 * m[15]));
			o[11] = d * ((t17 * m[3] + t20 * m[7] + t23 * m[11]) - (t16 * m[3] + t21 * m[7] + t22 * m[11]));
			o[12] = d * ((t14 * m[10] + t17 * m[14] + t13 * m[6]) - (t16 * m[14] + t12 * m[6] + t15 * m[10]));
			o[13] = d * ((t20 * m[14] + t12 * m[2] + t19 * m[10]) - (t18 * m[10] + t21 * m[14] + t13 * m[2]));
			o[14] = d * ((t18 * m[6] + t23 * m[14] + t15 * m[2]) - (t22 * m[14] + t14 * m[2] + t19 * m[6]));
			o[15] = d * ((t22 * m[10] + t16 * m[2] + t21 * m[6]) - (t20 * m[6] + t23 * m[10] + t17 * m[2]));

			return *this;
		}
		/// @brief Returns the inverse of the matrix.
		Matrix4x4 Inverse() const {
			//Big no touchie sign hire unless you know what this means, which I for sure don't
			const float32_t* m = data;

			float32_t t0 = m[10] * m[15];
			float32_t t1 = m[14] * m[11];
			float32_t t2 = m[6] * m[15];
			float32_t t3 = m[14] * m[7];
			float32_t t4 = m[6] * m[11];
			float32_t t5 = m[10] * m[7];
			float32_t t6 = m[2] * m[15];
			float32_t t7 = m[14] * m[3];
			float32_t t8 = m[2] * m[11];
			float32_t t9 = m[10] * m[3];
			float32_t t10 = m[2] * m[7];
			float32_t t11 = m[6] * m[3];
			float32_t t12 = m[8] * m[13];
			float32_t t13 = m[12] * m[9];
			float32_t t14 = m[4] * m[13];
			float32_t t15 = m[12] * m[5];
			float32_t t16 = m[4] * m[9];
			float32_t t17 = m[8] * m[5];
			float32_t t18 = m[0] * m[13];
			float32_t t19 = m[12] * m[1];
			float32_t t20 = m[0] * m[9];
			float32_t t21 = m[8] * m[1];
			float32_t t22 = m[0] * m[5];
			float32_t t23 = m[4] * m[1];

			Matrix4x4 outMatrix;
			float32_t* o = outMatrix.data;

			o[0] = (t0 * m[5] + t3 * m[9] + t4 * m[13]) - (t1 * m[5] + t2 * m[9] + t5 * m[13]);
			o[1] = (t1 * m[1] + t6 * m[9] + t9 * m[13]) - (t0 * m[1] + t7 * m[9] + t8 * m[13]);
			o[2] = (t2 * m[1] + t7 * m[5] + t10 * m[13]) - (t3 * m[1] + t6 * m[5] + t11 * m[13]);
			o[3] = (t5 * m[1] + t8 * m[5] + t11 * m[9]) - (t4 * m[1] + t9 * m[5] + t10 * m[9]);

			float32_t d = 1.0f / (m[0] * o[0] + m[4] * o[1] + m[8] * o[2] + m[12] * o[3]);

			o[0] = d * o[0];
			o[1] = d * o[1];
			o[2] = d * o[2];
			o[3] = d * o[3];
			o[4] = d * ((t1 * m[4] + t2 * m[8] + t5 * m[12]) - (t0 * m[4] + t3 * m[8] + t4 * m[12]));
			o[5] = d * ((t0 * m[0] + t7 * m[8] + t8 * m[12]) - (t1 * m[0] + t6 * m[8] + t9 * m[12]));
			o[6] = d * ((t3 * m[0] + t6 * m[4] + t11 * m[12]) - (t2 * m[0] + t7 * m[4] + t10 * m[12]));
			o[7] = d * ((t4 * m[0] + t9 * m[4] + t10 * m[8]) - (t5 * m[0] + t8 * m[4] + t11 * m[8]));
			o[8] = d * ((t12 * m[7] + t15 * m[11] + t16 * m[15]) - (t13 * m[7] + t14 * m[11] + t17 * m[15]));
			o[9] = d * ((t13 * m[3] + t18 * m[11] + t21 * m[15]) - (t12 * m[3] + t19 * m[11] + t20 * m[15]));
			o[10] = d * ((t14 * m[3] + t19 * m[7] + t22 * m[15]) - (t15 * m[3] + t18 * m[7] + t23 * m[15]));
			o[11] = d * ((t17 * m[3] + t20 * m[7] + t23 * m[11]) - (t16 * m[3] + t21 * m[7] + t22 * m[11]));
			o[12] = d * ((t14 * m[10] + t17 * m[14] + t13 * m[6]) - (t16 * m[14] + t12 * m[6] + t15 * m[10]));
			o[13] = d * ((t20 * m[14] + t12 * m[2] + t19 * m[10]) - (t18 * m[10] + t21 * m[14] + t13 * m[2]));
			o[14] = d * ((t18 * m[6] + t23 * m[14] + t15 * m[2]) - (t22 * m[14] + t14 * m[2] + t19 * m[6]));
			o[15] = d * ((t22 * m[10] + t16 * m[2] + t21 * m[6]) - (t20 * m[6] + t23 * m[10] + t17 * m[2]));

			return outMatrix;
		}
		/// @brief Transposes the matrix.
		Matrix4x4& Transpose() {
			float32_t prevData[16];
			memcpy(prevData, data, sizeof(float32_t) * 16);

			data[0] = prevData[0];
			data[1] = prevData[4];
			data[2] = prevData[8];
			data[3] = prevData[12];
			data[4] = prevData[1];
			data[5] = prevData[5];
			data[6] = prevData[9];
			data[7] = prevData[13];
			data[8] = prevData[2];
			data[9] = prevData[6];
			data[10] = prevData[10];
			data[11] = prevData[14];
			data[12] = prevData[3];
			data[13] = prevData[7];
			data[14] = prevData[11];
			data[15] = prevData[15];

			return *this;
		}
		/// @brief Returns the transposed version of the matrix.
		Matrix4x4 Transposed() const {
			Matrix4x4 outMatrix;
			
			outMatrix.data[0] = data[0];
			outMatrix.data[1] = data[4];
			outMatrix.data[2] = data[8];
			outMatrix.data[3] = data[12];
			outMatrix.data[4] = data[1];
			outMatrix.data[5] = data[5];
			outMatrix.data[6] = data[9];
			outMatrix.data[7] = data[13];
			outMatrix.data[8] = data[2];
			outMatrix.data[9] = data[6];
			outMatrix.data[10] = data[10];
			outMatrix.data[11] = data[14];
			outMatrix.data[12] = data[3];
			outMatrix.data[13] = data[7];
			outMatrix.data[14] = data[11];
			outMatrix.data[15] = data[15];

			return outMatrix;
		}

		/// @brief Returns the forward vector of the matrix.
		Vector3 Forward()  const { return Vector3(-data[2], -data[6], -data[10]).Normalized(); }
		/// @brief Returns the backward vector of the matrix.
		Vector3 Backward() const { return Vector3( data[2],  data[6],  data[10]).Normalized(); }
		/// @brief Returns the up vector of the matrix.
		Vector3 Up()       const { return Vector3( data[1],  data[5],  data[9] ).Normalized(); }
		/// @brief Returns the down vector of the matrix.
		Vector3 Down()     const { return Vector3(-data[1], -data[5], -data[9] ).Normalized(); }
		/// @brief Returns the left vector of the matrix.
		Vector3 Left()     const { return Vector3(-data[0], -data[4], -data[8] ).Normalized(); }
		/// @brief Returns the right vector of the matrix.
		Vector3 Right()    const { return Vector3( data[0],  data[4],  data[8] ).Normalized(); }

		/// @brief Returns an ortographic projection matrix. Usually used to render 2D scenes.
		/// 
		/// @param left The left size of the view frustum.
		/// @param right The right size of the view frustum.
		/// @param bottom The bottom size of the view frustum.
		/// @param top The top size of the view frustum.
		/// @param nearPlane The near clipping plane distance.
		/// @param farPlane The far clipping plane distance.
		static Matrix4x4 OrtographicProjection(float32_t left, float32_t right, float32_t bottom, float32_t top, float32_t nearPlane, float32_t farPlane) {
			Matrix4x4 outMatrix;

			float32_t lr = 1.f / (left - right);
			float32_t bt = 1.f / (bottom - top);
			float32_t nf = 1.f / (nearPlane - farPlane);

			outMatrix.data[0] = -2.f * lr;
			outMatrix.data[5] = -2.f * bt;
			outMatrix.data[10] = 2.f * nf;

			outMatrix.data[12] = (left + right) * lr;
			outMatrix.data[13] = (top + bottom) * bt;
			outMatrix.data[14] = (farPlane + nearPlane) * nf;

			return outMatrix;
		}
		/// @brief Returns a perspective projection matrix. Usually used to render 3D scenes.
		///
		/// @param fov The field of view in radians.
		/// @param aspectRatio The aspect ratio.
		/// @param nearPlane The near clipping plane distance.
		/// @param farPlane The far clipping plane distance.
		static Matrix4x4 PerspectiveProjection(float32_t fov, float32_t aspectRatio, float32_t nearPlane, float32_t farPlane) {
			float32_t halfTanfFov = tanf(fov * 0.5f);

			Matrix4x4 outMatrix;

			outMatrix.data[0] = 1.f / (aspectRatio * halfTanfFov);
			outMatrix.data[5] = 1.f / halfTanfFov;
			outMatrix.data[10] = -farPlane / (farPlane - nearPlane);
			outMatrix.data[11] = -1.f;
			outMatrix.data[14] = -(nearPlane * farPlane) / (farPlane - nearPlane);
			outMatrix.data[15] = 0.f;
			return outMatrix;
		}
		/// @brief Returns a look-at matrix, or a matrix looking at target from the perspective of position.
		///
		/// @param position The position of the matrix.
		/// @param target The position to "look at".
		/// @param up The up vector.
		static Matrix4x4 LookAt(Vector3 position, Vector3 target, Vector3 up) {
			Matrix4x4 outMatrix;

			Vector3 zAxis = target - position;
			zAxis.Normalize();

			Vector3 xAxis = zAxis.Cross(up).Normalized();
			Vector3 yAxis = xAxis.Cross(zAxis);

			outMatrix.data[0] = xAxis.x;
			outMatrix.data[1] = yAxis.x;
			outMatrix.data[2] = -zAxis.x;

			outMatrix.data[4] = xAxis.y;
			outMatrix.data[5] = yAxis.y;
			outMatrix.data[6] = -zAxis.y;

			outMatrix.data[8] = xAxis.z;
			outMatrix.data[9] = yAxis.z;
			outMatrix.data[10] = -zAxis.z;

			outMatrix.data[12] = -xAxis.Dot(position);
			outMatrix.data[13] = -yAxis.Dot(position);
			outMatrix.data[14] = zAxis.Dot(position);

			return outMatrix;
		}
		/// @brief Returns a translation matrix, or a movement matrix.
		///
		/// @param position The matrix position.
		static Matrix4x4 Translation(Vector3 position) {
			Matrix4x4 outMatrix;
			outMatrix.data[12] = position.x;
			outMatrix.data[13] = position.y;
			outMatrix.data[14] = position.z;
			return outMatrix;
		}
		/// @brief Returns a rotation matrix for rotation around the X axis.
		/// 
		/// @param angle The angle in radians.
		static Matrix4x4 EulerX(float32_t angle) {
			Matrix4x4 outMatrix;
			float32_t s = sinf(angle), c = cosf(angle);

			outMatrix.data[5] = c;
			outMatrix.data[6] = s;
			outMatrix.data[9] = -s;
			outMatrix.data[10] = c;
			return outMatrix;
		}
		/// @brief Returns a rotation matrix for rotation around the Y axis.
		/// 
		/// @param angle The angle in radians.
		static Matrix4x4 EulerY(float32_t angle) {
			Matrix4x4 outMatrix;
			float32_t s = sinf(angle), c = cosf(angle);

			outMatrix.data[0] = c;
			outMatrix.data[2] = -s;
			outMatrix.data[8] = s;
			outMatrix.data[10] = c;
			return outMatrix;
		}
		/// @brief Returns a rotation matrix for rotation around the Z axis.
		/// 
		/// @param angle The angle in radians.
		static Matrix4x4 EulerZ(float32_t angle) {
			Matrix4x4 outMatrix;
			float32_t s = sinf(angle), c = cosf(angle);

			outMatrix.data[5] = c;
			outMatrix.data[6] = s;
			outMatrix.data[9] = -s;
			outMatrix.data[10] = c;
			return outMatrix;
		}
		/// @brief Returns a rotation matrix for Euler rotation.
		/// 
		/// @param x The angle around the X axis in radians.
		/// @param y The angle around the Y axis in radians.
		/// @param z The angle around the Z axis in radians.
		static Matrix4x4 EulerXYZ(float32_t x, float32_t y, float32_t z) {
			return EulerZ(z) * EulerX(x) * EulerY(y);
		}
		/// @brief Returns a rotation matrix from a quaternion.
		/// 
		/// @param rotation The quaternion used for rotation.
		static Matrix4x4 Rotation(Quaternion rotation) {
			Matrix4x4 outMatrix;

			rotation.Normalize();
			Quaternion normalized = rotation.Normalized();

			outMatrix.data[0]  = 1.f - 2.f * rotation.y * rotation.y - 2.f * rotation.z * rotation.z;
			outMatrix.data[1]  =       2.f * rotation.x * rotation.y - 2.f * rotation.z * rotation.w;
			outMatrix.data[2]  =       2.f * rotation.x * rotation.z + 2.f * rotation.y * rotation.w;

			outMatrix.data[4]  =       2.f * rotation.x * rotation.y + 2.f * rotation.z * rotation.w;
			outMatrix.data[5]  = 1.f - 2.f * rotation.x * rotation.x - 2.f * rotation.z * rotation.z;
			outMatrix.data[6]  =       2.f * rotation.y * rotation.z - 2.f * rotation.x * rotation.w;

			outMatrix.data[8]  =       2.f * rotation.x * rotation.z - 2.f * rotation.y * rotation.w;
			outMatrix.data[9]  =       2.f * rotation.y * rotation.z + 2.f * rotation.x * rotation.w;
			outMatrix.data[10] = 1.f - 2.f * rotation.x * rotation.x - 2.f * rotation.y * rotation.y;

			return outMatrix;
		}
		/// @brief Returns a scaling matrix.
		/// 
		/// @param scale The scale of the matrix.
		static Matrix4x4 Scaling(Vector3 scale) {
			Matrix4x4 outMatrix;

			outMatrix.data[0] = scale.x;
			outMatrix.data[5] = scale.y;
			outMatrix.data[10] = scale.z;

			return outMatrix;
		}

		~Matrix4x4() = default;
	};
}