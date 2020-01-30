#ifndef __MATRIX_H__
#define __MATRIX_H__

namespace math
{
	template<typename T>
	class Matrix
	{
	public:
		Matrix<T>(int matrixType = 1);
		Matrix<T>(T data[4][4]);
		Matrix<T>(const Matrix<T>& mat);
		void zero();
		void identity();
		void translate(const T& x, const T& y, const T& z);
		void scale(const T& x, const T& y, const T& z);
		void rotate(const T& x, const T& y, const T& z);
		Matrix<T> transpose() const;
		Matrix<T> inverse() const;
		bool isIdentity() const;

		Matrix<T>& operator=(const Matrix<T>& mat);
		Matrix<T> operator+(const Matrix<T>& mat);
		Matrix<T> operator-(const Matrix<T>& mat);
		Matrix<T> operator*(const Matrix<T>& mat);
		Matrix<T> operator*(const T& v);
		bool operator==(const Matrix<T>& mat);
		bool operator!=(const Matrix<T>& mat);

	public:
		T value[16];
	};
}

#endif