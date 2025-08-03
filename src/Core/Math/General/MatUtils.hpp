#pragma once

#include "Core/Math/Templates/Mat.hpp"
#include "Core/Math/Templates/Quat.hpp"
#include "Core/Math/Templates/Vec3.hpp"

namespace wfe {
	/// @brief The 2x2 identity matrix, using single precision floating point values.
	constexpr Mat2x2f MAT2X2F_IDENTITY {
		1.0f, 0.0f,
		0.0f, 1.0f
	};
	/// @brief The 3x3 identity matrix, using single precision floating point values.
	constexpr Mat3x3f MAT3X3F_IDENTITY {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
	/// @brief The 4x4 identity matrix, using single precision floating point values.
	constexpr Mat4x4f MAT4X4F_IDENTITY {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	/// @brief The 2x2 identity matrix, using double precision floating point values.
	constexpr Mat2x2d MAT2X2D_IDENTITY {
		1.0, 0.0,
		0.0, 1.0
	};
	/// @brief The 3x3 identity matrix, using double precision floating point values.
	constexpr Mat3x3d MAT3X3D_IDENTITY {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0
	};
	/// @brief The 4x4 identity matrix, using double precision floating point values.
	constexpr Mat4x4d MAT4X4D_IDENTITY {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	/// @brief The 2x2 identity matrix, using unsigned integer values.
	constexpr Mat2x2u MAT2X2U_IDENTITY {
		1, 0,
		0, 1
	};
	/// @brief The 3x3 identity matrix, using unsigned integer values.
	constexpr Mat3x3u MAT3X3U_IDENTITY {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	};
	/// @brief The 4x4 identity matrix, using unsigned integer values.
	constexpr Mat4x4u MAT4X4U_IDENTITY {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	/// @brief The 2x2 identity matrix, using signed integer values.
	constexpr Mat2x2i MAT2X2I_IDENTITY {
		1, 0,
		0, 1
	};
	/// @brief The 3x3 identity matrix, using signed integer values.
	constexpr Mat3x3i MAT3X3I_IDENTITY {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	};
	/// @brief The 4x4 identity matrix, using signed integer values.
	constexpr Mat4x4i MAT4X4I_IDENTITY {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	/// @brief Transposes the given matrix.
	/// @tparam N The number of rows in the given matrix.
	/// @tparam M The number of columns in the given matrix.
	/// @tparam T The numerical type used by the matrix.
	/// @param mat The matrix to transpose.
	/// @return The transposed version of the given matrix.
	template<size_t N, size_t M, class T>
	inline Mat<M, N, T> MatTranspose(const Mat<N, M, T>& mat) {
		Mat<M, N, T> res;
		for(size_t i = 0; i != M; ++i) {
			for(size_t j = 0; j != N; ++j)
				res.data[i][j] = mat.data[j][i];
		}
		return res;
	}

	/// @brief Builds a 4x4 transform matrix representing a 3D translation.
	/// @param pos The position to translate to.
	/// @return The resulting transform matrix.
	inline Mat4x4f Mat4x4Translate(const Vec3f& pos) {
		return {
			1.0f, 0.0f, 0.0f, pos.x,
			0.0f, 1.0f, 0.0f, pos.y,
			0.0f, 0.0f, 1.0f, pos.z,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}
	/// @brief Builds a 4x4 transform matrix representing a 3D translation.
	/// @param pos The position to translate to.
	/// @return The resulting transform matrix.
	inline Mat4x4d Mat4x4Translate(const Vec3d& pos) {
		return {
			1.0, 0.0, 0.0, pos.x,
			0.0, 1.0, 0.0, pos.y,
			0.0, 0.0, 1.0, pos.z,
			0.0, 0.0, 0.0, 1.0
		};
	}
	/// @brief Builds a 4x4 transform matrix representing a 3D rotation.
	/// @param rot The unit quaternion representing the target rotation.
	/// @return The resulting transform matrix.
	inline Mat4x4f Mat4x4Rotate(const Quatf& rot) {
		return {
			1.0f - 2.0f * (rot.y * rot.y + rot.z * rot.z),        2.0f * (rot.x * rot.y - rot.z * rot.w),        2.0f * (rot.x * rot.z + rot.y * rot.w), 0.0f,
			       2.0f * (rot.x * rot.y + rot.z * rot.w), 1.0f - 2.0f * (rot.x * rot.x + rot.z * rot.z),        2.0f * (rot.y * rot.z - rot.x * rot.w), 0.0f,
			       2.0f * (rot.x * rot.z - rot.y * rot.w),        2.0f * (rot.y * rot.z + rot.x * rot.w), 1.0f - 2.0f * (rot.x * rot.x + rot.y * rot.y), 0.0f,
			0.0f,                                          0.0f,                                          0.0f,                                          1.0f
		};
	}
	/// @brief Builds a 4x4 transform matrix representing a 3D rotation.
	/// @param rot The unit quaternion representing the target rotation.
	/// @return The resulting transform matrix.
	inline Mat4x4d Mat4x4Rotate(const Quatd& rot) {
		return {
			1.0 - 2.0 * (rot.y * rot.y + rot.z * rot.z),       2.0 * (rot.x * rot.y - rot.z * rot.w),       2.0 * (rot.x * rot.z + rot.y * rot.w), 0.0,
			      2.0 * (rot.x * rot.y + rot.z * rot.w), 1.0 - 2.0 * (rot.x * rot.x + rot.z * rot.z),       2.0 * (rot.y * rot.z - rot.x * rot.w), 0.0,
			      2.0 * (rot.x * rot.z - rot.y * rot.w),       2.0 * (rot.y * rot.z + rot.x * rot.w), 1.0 - 2.0 * (rot.x * rot.x + rot.y * rot.y), 0.0,
			0.0,                                         0.0,                                         0.0,                                         1.0
		};
	}
	/// @brief Builds a 4x4 transform matrix representing a 3D scaling.
	/// @param scale The vector representing the scaling in every dimension.
	/// @return The resulting transform matrix.
	inline Mat4x4f Mat4x4Scale(const Vec3f& scale) {
		return {
			scale.x, 0.0f,    0.0f,    0.0f,
			0.0f,    scale.y, 0.0f,    0.0f,
			0.0f,    0.0f,    scale.z, 0.0f,
			0.0f,    0.0f,    0.0f,    1.0f
		};
	}
	/// @brief Builds a 4x4 transform matrix representing a 3D scaling.
	/// @param scale The vector representing the scaling in every dimension.
	/// @return The resulting transform matrix.
	inline Mat4x4d Mat4x4Scale(const Vec3d& scale) {
		return {
			scale.x, 0.0,     0.0,     0.0,
			0.0,     scale.y, 0.0,     0.0,
			0.0,     0.0,     scale.z, 0.0,
			0.0,     0.0,     0.0,     1.0
		};
	}
	/// @brief Builds a 4x4 transform matrix representing perspective projection.
	/// @param fov The camera's vertical field of view, in radians.
	/// @param aspectRatio The ratio between the camera's width and height.
	/// @param nearPlane The distance from the origin point to the near clipping plane.
	/// @param farPlane The distance from the origin point to the far clipping plane.
	/// @return The resulting transform matrix.
	inline Mat4x4f Mat4x4PerspectiveProjection(float fov, float aspectRatio, float nearPlane, float farPlane) {
		// Precalculate values to reduce operation count
		float halfFovTan = Tan(fov * 0.5f);
		float invNearFar = 1.0f / (nearPlane - farPlane);

		// Build the matrix
		return {
			1.0f / (aspectRatio * halfFovTan),  0.0f,               0.0f,                  0.0f,
			0.0f,                              -1.0f / halfFovTan,  0.0f,                  0.0f,
			0.0f,                               0.0f,               farPlane * invNearFar, farPlane * nearPlane * invNearFar,
			0.0f,                               0.0f,              -1.0f,                  0.0f
		};
	}
	/// @brief Builds a 4x4 transform matrix representing perspective projection.
	/// @param fov The camera's vertical field of view, in radians.
	/// @param aspectRatio The ratio between the camera's width and height.
	/// @param nearPlane The distance from the origin point to the near clipping plane.
	/// @param farPlane The distance from the origin point to the far clipping plane.
	/// @return The resulting transform matrix.
	inline Mat4x4d Mat4x4PerspectiveProjection(double fov, double aspectRatio, double nearPlane, double farPlane) {
		// Precalculate values to reduce operation count
		double halfFovTan = Tan(fov * 0.5);
		double invNearFar = 1.0 / (nearPlane - farPlane);

		// Build the matrix
		return {
			1.0 / (aspectRatio * halfFovTan),  0.0,               0.0,                   0.0,
			0.0,                              -1.0 / halfFovTan,  0.0,                   0.0,
			0.0,                               0.0,               farPlane * invNearFar, farPlane * nearPlane * invNearFar,
			0.0,                               0.0,              -1.0,                   0.0
		};
	}
	/// @brief Builds a 4x4 transform matrix representing ortographic projection.
	/// @param left The left size of the view frustum.
	/// @param right The right size of the view frustum.
	/// @param bottom The bottom size of the view frustum.
	/// @param top The top size of the view frustum.
	/// @param nearPlane The distance from the origin point to the near clipping plane.
	/// @param farPlane The distance from the origin point to the far clipping plane.
	/// @return The resulting transform matrix.
	inline Mat4x4f Mat4x4OrtographicProjection(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
		// Precalculate values to reduce operation count
		float invLeftRight = 1.0f / (left - right);
		float invBottomTop = 1.0f / (bottom - top);
		float invNearFar = 1.0f / (nearPlane - farPlane);

		// Build the matrix
		return {
			-2.0f * invLeftRight, 0.0f,                0.0f,        (left + right) * invLeftRight,
			 0.0f,                2.0f * invBottomTop, 0.0f,       -(bottom + top) * invBottomTop,
			 0.0f,                0.0f,                invNearFar, -nearPlane * invNearFar,
			 0.0f,                0.0f,                0.0f,        1.0f
		};
	}
	/// @brief Builds a 4x4 transform matrix representing ortographic projection.
	/// @param left The left size of the view frustum.
	/// @param right The right size of the view frustum.
	/// @param bottom The bottom size of the view frustum.
	/// @param top The top size of the view frustum.
	/// @param nearPlane The distance from the origin point to the near clipping plane.
	/// @param farPlane The distance from the origin point to the far clipping plane.
	/// @return The resulting transform matrix.
	inline Mat4x4d Mat4x4OrtographicProjection(double left, double right, double bottom, double top, double nearPlane, double farPlane) {
		// Precalculate values to reduce operation count
		double invLeftRight = 1.0 / (left - right);
		double invBottomTop = 1.0 / (bottom - top);
		double invNearFar = 1.0 / (nearPlane - farPlane);

		// Build the matrix
		return {
			-2.0 * invLeftRight, 0.0,                0.0,         (left + right) * invLeftRight,
			 0.0,                2.0 * invBottomTop, 0.0,        -(bottom + top) * invBottomTop,
			 0.0,                0.0,                invNearFar, -nearPlane * invNearFar,
			 0.0,                0.0,                0.0,         1.0
		};
	}
}