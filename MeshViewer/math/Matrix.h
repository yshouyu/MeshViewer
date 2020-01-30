#ifndef __MATRIX_H__
#define __MATRIX_H__

namespace math
{
	class Matrix
	{
	public:
		Matrix(int matrixType = 1);
		Matrix(float data[4][4]);
		Matrix(const Matrix& mat);
		void zero();
		void identity();
		void translate(const float& x, const float& y, const float& z);
		void scale(const float& x, const float& y, const float& z);
		void rotate(const float& x, const float& y, const float& z);
		Matrix transpose() const;
		Matrix inverse() const;
		bool isIdentity() const;

		Matrix& operator=(const Matrix& mat);
		Matrix operator+(const Matrix& mat);
		Matrix operator-(const Matrix& mat);
		Matrix operator*(const Matrix& mat);
		Matrix operator*(const float& v);
		bool operator==(const Matrix& mat);
		bool operator!=(const Matrix& mat);

	public:
		float value[16];
	};
}

#endif