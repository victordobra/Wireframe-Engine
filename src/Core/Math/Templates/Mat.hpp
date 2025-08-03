#pragma once

#include "Vec.hpp"
#include "Core/Types/Defines.hpp"

namespace wfe {
	/// @brief A general template for a mathematical NxM row-major matrix, using the numerical type T.
	/// @tparam N The number of rows in the matrix.
	/// @tparam M The number of columns in the matrix.
	/// @tparam T The numerical type of the matrix's data.
	template<size_t N, size_t M, class T>
	class Mat {
	public:
		/// @brief The matrix's data.
		T data[N][M]{};

		/// @brief Gets the row with the given index.
		/// @param index The row's index.
		/// @return A pointer to the requested row.
		constexpr T* operator[](size_t index) {
			return data[index];
		}
		/// @brief Gets the row with the given index.
		/// @param index The row's index.
		/// @return A pointer to the requested row.
		constexpr const T* operator[](size_t index) const {
			return data[index];
		}

		/// @brief Adds the given matrix to this matrix.
		/// @param other The matix to add.
		/// @return A reference to this matrix.
		constexpr Mat& operator+=(const Mat& other) {
			for(size_t i = 0; i != N; ++i) {
				for(size_t j = 0; j != M; ++j)
					data[i][j] += other.data[i][j];
			}

			return *this;
		}
		/// @brief Subtracts the given matrix from this matrix.
		/// @param other The matix to subtract.
		/// @return A reference to this matrix.
		constexpr Mat& operator-=(const Mat& other) {
			for(size_t i = 0; i != N; ++i) {
				for(size_t j = 0; j != M; ++j)
					data[i][j] -= other.data[i][j];
			}

			return *this;
		}
		/// @brief Multiplies this matrix by the given scalar.
		/// @param other The scalar to multiply by.
		/// @return A reference to this matrix.
		constexpr Mat& operator*=(const T& other) {
			for(size_t i = 0; i != N; ++i) {
				for(size_t j = 0; j != M; ++j)
					data[i][j] *= other;
			}

			return *this;
		}
		/// @brief Divides this matrix by the given scalar.
		/// @param other The scalar to divide by.
		/// @return A reference to this matrix.
		constexpr Mat& operator/=(const T& other) {
			for(size_t i = 0; i != N; ++i) {
				for(size_t j = 0; j != M; ++j)
					data[i][j] /= other;
			}

			return *this;
		}
	};

	/// @brief Checks if two matrices are equal.
	/// @tparam N The number of rows in the matrices.
	/// @tparam M The number of columns in the matrices.
	/// @tparam T The numerical type of the matrices' components.
	/// @param mat1 The first matrix to compare.
	/// @param mat2 The second matrix to compare.
	/// @return True if the two matrices are equal, otherwise false.
	template<size_t N, size_t M, class T>
	inline constexpr bool operator==(const Mat<N, M, T>& mat1, const Mat<N, M, T>& mat2) {
		for(size_t i = 0; i != N; ++i) {
			for(size_t j = 0; j != M; ++j) {
				if(mat1.data[i][j] != mat2.data[i][j])
					return false;
			}
		}

		return true;
	}
	/// @brief Checks if two matrices are different.
	/// @tparam N The number of rows in the matrix.
	/// @tparam M The number of columns in the matrix.
	/// @tparam T The numerical type of the matrix's components.
	/// @param mat1 The first matrix to compare.
	/// @param mat2 The second matrix to compare.
	/// @return True if the two matrices are different, otherwise false.
	template<size_t N, size_t M, class T>
	inline constexpr bool operator!=(const Mat<N, M, T>& mat1, const Mat<N, M, T>& mat2) {
		for(size_t i = 0; i != N; ++i) {
			for(size_t j = 0; j != M; ++j) {
				if(mat1.data[i][j] != mat2.data[i][j])
					return true;
			}
		}

		return false;
	}

	/// @brief Adds two matrices.
	/// @tparam N The number of rows in the matrix.
	/// @tparam M The number of columns in the matrix.
	/// @tparam T The numerical type of the matrix's components.
	/// @param mat1 The first matrix to add.
	/// @param mat2 The second matrix to add.
	/// @return The resulting matrix.
	template<size_t N, size_t M, class T>
	inline constexpr Mat<N, M, T> operator+(const Mat<N, M, T>& mat1, const Mat<N, M, T>& mat2) {
		Mat<N, M, T> res;
		for(size_t i = 0; i != N; ++i) {
			for(size_t j = 0; j != M; ++j)
				res.data[i][j] = mat1.data[i][j] + mat2.data[i][j];
		}

		return res;
	}
	/// @brief Subtracts two matrices.
	/// @tparam N The number of rows in the matrix.
	/// @tparam M The number of columns in the matrix.
	/// @tparam T The numerical type of the matrix's components.
	/// @param mat1 The matrix to subtract from.
	/// @param mat2 The matrix to subtract.
	/// @return The resulting matrix.
	template<size_t N, size_t M, class T>
	inline constexpr Mat<N, M, T> operator-(const Mat<N, M, T>& mat1, const Mat<N, M, T>& mat2) {
		Mat<N, M, T> res;
		for(size_t i = 0; i != N; ++i) {
			for(size_t j = 0; j != M; ++j)
				res.data[i][j] = mat1.data[i][j] - mat2.data[i][j];
		}

		return res;
	}
	/// @brief Multiplies a matrix by a scalar value.
	/// @tparam N The number of rows in the matrix.
	/// @tparam M The number of columns in the matrix.
	/// @tparam T The numerical type of the matrix's components.
	/// @param mat The matrix to multiply.
	/// @param scalar The scalar to multiply by.
	/// @return The resulting matrix.
	template<size_t N, size_t M, class T>
	inline constexpr Mat<N, M, T> operator*(const Mat<N, M, T>& mat, const T& scalar) {
		Mat<N, M, T> res;
		for(size_t i = 0; i != N; ++i) {
			for(size_t j = 0; j != M; ++j)
				res.data[i][j] = mat.data[i][j] * scalar;
		}

		return res;
	}
	/// @brief Multiplies a matrix by a scalar value.
	/// @tparam N The number of rows in the matrix.
	/// @tparam M The number of columns in the matrix.
	/// @tparam T The numerical type of the matrix's components.
	/// @param scalar The scalar to multiply by.
	/// @param mat The matrix to multiply.
	/// @return The resulting matrix.
	template<size_t N, size_t M, class T>
	inline constexpr Mat<N, M, T> operator*(const T& scalar, const Mat<N, M, T>& mat) {
		Mat<N, M, T> res;
		for(size_t i = 0; i != N; ++i) {
			for(size_t j = 0; j != M; ++j)
				res.data[i][j] = scalar * mat.data[i][j];
		}

		return res;
	}
	/// @brief Multiplies a matrix by a vector.
	/// @tparam N The number of rows in the matrix.
	/// @tparam M The number of columns in the matrix.
	/// @tparam T The numerical type of the matrix's components.
	/// @param mat The matrix to multiply.
	/// @param vec The vector to multiply.
	/// @return The resulting vector.
	template<size_t N, size_t M, class T>
	inline constexpr Vec<N, T> operator*(const Mat<N, M, T>& mat, const Vec<M, T>& vec) {
		Vec<N, T> res;
		for(size_t i = 0; i != N; ++i) {
			for(size_t j = 0; j != M; ++j)
				res[i] += mat.data[i][j] * vec[j];
		}

		return res;
	}
	/// @brief Multiplies a vector by a matrix.
	/// @tparam N The number of rows in the matrix.
	/// @tparam M The number of columns in the matrix.
	/// @tparam T The numerical type of the matrix's components.
	/// @param vec The vector to multiply.
	/// @param mat The matrix to multiply.
	/// @return The resulting vector.
	template<size_t N, size_t M, class T>
	inline constexpr Vec<M, T> operator*(const Vec<N, T>& vec, const Mat<N, M, T>& mat) {
		Vec<M, T> res;
		for(size_t i = 0; i != M; ++i) {
			for(size_t j = 0; j != N; ++j)
				res[i] += mat.data[j][i] * vec[j];
		}

		return res;
	}
	template<size_t N, size_t M, size_t P, class T> 
	inline constexpr Mat<N, P, T> operator*(const Mat<N, M, T>& mat1, const Mat<M, P, T>& mat2) {
		Mat<N, P, T> res;
		for(size_t i = 0; i != N; ++i) {
			for(size_t j = 0; j != P; ++j) {
				for(size_t k = 0; k != M; ++k)
					res.data[i][j] += mat1.data[i][k] * mat2.data[k][j];
			}
		}

		return res;
	}
	/// @brief Divides a matrix by a scalar value.
	/// @tparam N The number of rows in the matrix.
	/// @tparam M The number of columns in the matrix.
	/// @tparam T The numerical type of the matrix's components.
	/// @param mat The matrix to divide.
	/// @param scalar The scalar to divide by.
	/// @return The resulting matrix.
	template<size_t N, size_t M, class T>
	inline constexpr Mat<N, M, T> operator/(const Mat<N, M, T>& mat, const T& scalar) {
		Mat<N, M, T> res;
		for(size_t i = 0; i != N; ++i) {
			for(size_t j = 0; j != M; ++j)
				res.data[i][j] = mat.data[i][j] / scalar;
		}

		return res;
	}

	/// @brief The 2x2 matrix type using single precision floating point values.
	typedef Mat<2, 2, float> Mat2x2f;
	/// @brief The 2x3 matrix type using single precision floating point values.
	typedef Mat<2, 3, float> Mat2x3f;
	/// @brief The 2x4 matrix type using single precision floating point values.
	typedef Mat<2, 4, float> Mat2x4f;
	/// @brief The 3x2 matrix type using single precision floating point values.
	typedef Mat<3, 2, float> Mat3x2f;
	/// @brief The 3x3 matrix type using single precision floating point values.
	typedef Mat<3, 3, float> Mat3x3f;
	/// @brief The 3x4 matrix type using single precision floating point values.
	typedef Mat<3, 4, float> Mat3x4f;
	/// @brief The 4x2 matrix type using single precision floating point values.
	typedef Mat<4, 2, float> Mat4x2f;
	/// @brief The 4x3 matrix type using single precision floating point values.
	typedef Mat<4, 3, float> Mat4x3f;
	/// @brief The 4x4 matrix type using single precision floating point values.
	typedef Mat<4, 4, float> Mat4x4f;

	/// @brief The 2x2 matrix type using double precision floating point values.
	typedef Mat<2, 2, double> Mat2x2d;
	/// @brief The 2x3 matrix type using double precision floating point values.
	typedef Mat<2, 3, double> Mat2x3d;
	/// @brief The 2x4 matrix type using double precision floating point values.
	typedef Mat<2, 4, double> Mat2x4d;
	/// @brief The 3x2 matrix type using double precision floating point values.
	typedef Mat<3, 2, double> Mat3x2d;
	/// @brief The 3x3 matrix type using double precision floating point values.
	typedef Mat<3, 3, double> Mat3x3d;
	/// @brief The 3x4 matrix type using double precision floating point values.
	typedef Mat<3, 4, double> Mat3x4d;
	/// @brief The 4x2 matrix type using double precision floating point values.
	typedef Mat<4, 2, double> Mat4x2d;
	/// @brief The 4x3 matrix type using double precision floating point values.
	typedef Mat<4, 3, double> Mat4x3d;
	/// @brief The 4x4 matrix type using double precision floating point values.
	typedef Mat<4, 4, double> Mat4x4d;

	/// @brief The 2x2 matrix type using unsigned integer values.
	typedef Mat<2, 2, uint32_t> Mat2x2u;
	/// @brief The 2x3 matrix type using unsigned integer values.
	typedef Mat<2, 3, uint32_t> Mat2x3u;
	/// @brief The 2x4 matrix type using unsigned integer values.
	typedef Mat<2, 4, uint32_t> Mat2x4u;
	/// @brief The 3x2 matrix type using unsigned integer values.
	typedef Mat<3, 2, uint32_t> Mat3x2u;
	/// @brief The 3x3 matrix type using unsigned integer values.
	typedef Mat<3, 3, uint32_t> Mat3x3u;
	/// @brief The 3x4 matrix type using unsigned integer values.
	typedef Mat<3, 4, uint32_t> Mat3x4u;
	/// @brief The 4x2 matrix type using unsigned integer values.
	typedef Mat<4, 2, uint32_t> Mat4x2u;
	/// @brief The 4x3 matrix type using unsigned integer values.
	typedef Mat<4, 3, uint32_t> Mat4x3u;
	/// @brief The 4x4 matrix type using unsigned integer values.
	typedef Mat<4, 4, uint32_t> Mat4x4u;

	/// @brief The 2x2 matrix type using signed integer values.
	typedef Mat<2, 2, int32_t> Mat2x2i;
	/// @brief The 2x3 matrix type using signed integer values.
	typedef Mat<2, 3, int32_t> Mat2x3i;
	/// @brief The 2x4 matrix type using signed integer values.
	typedef Mat<2, 4, int32_t> Mat2x4i;
	/// @brief The 3x2 matrix type using signed integer values.
	typedef Mat<3, 2, int32_t> Mat3x2i;
	/// @brief The 3x3 matrix type using signed integer values.
	typedef Mat<3, 3, int32_t> Mat3x3i;
	/// @brief The 3x4 matrix type using signed integer values.
	typedef Mat<3, 4, int32_t> Mat3x4i;
	/// @brief The 4x2 matrix type using signed integer values.
	typedef Mat<4, 2, int32_t> Mat4x2i;
	/// @brief The 4x3 matrix type using signed integer values.
	typedef Mat<4, 3, int32_t> Mat4x3i;
	/// @brief The 4x4 matrix type using signed integer values.
	typedef Mat<4, 4, int32_t> Mat4x4i;
}

template<size_t N, size_t M, class T>
struct std::hash<wfe::Mat<N, M, T>> {
	std::size_t operator()(const wfe::Mat<N, M, T>& mat) {
		// Combine the hashes of all of the matrix's components
		std::hash<T> hasher;

		std::size_t res = 0;
		for(size_t i = 0; i != N; ++i) {
			for(size_t j = 0; j != M; ++j)
				res ^= hasher(mat.data[i][j]) + 0x9e3779b9 + (res << 6) + (res >> 2);
		}

		return res;
	}
};