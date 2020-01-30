#include "Matrix.h"

#include <math.h>

namespace math
{
	Matrix::Matrix(int matrixType)
	{
		if (matrixType == 0)
		{
			zero();
		}
		else
		{
			identity();
		}
	}

	Matrix::Matrix(float data[4][4])
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				value[i * 4 + j] = *data[i * 4 + j];
			}
		}
	}

	Matrix::Matrix(const Matrix& mat)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				value[i * 4 + j] = mat.value[i * 4 + j];
			}
		}
	}

	void Matrix::zero()
	{
		for (int i = 0; i < 16; i++)
		{
			value[i] = 0.0f;
		}
	}

	void Matrix::identity()
	{
		zero();
		value[0] = value[5] = value[10] = value[15] = 1.0f;
	}

	void Matrix::translate(const float& x, const float& y, const float& z)
	{
		value[12] = x;
		value[13] = y;
		value[14] = z;
	}

	void Matrix::scale(const float& x, const float& y, const float& z)
	{
		value[0] = x;
		value[5] = y;
		value[10] = z;
	}

	void Matrix::rotate(const float& x, const float& y, const float& z)
	{
		const double cr = cos(x);
		const double sr = sin(x);
		const double cp = cos(y);
		const double sp = sin(y);
		const double cy = cos(z);
		const double sy = sin(z);
		value[0] = static_cast<float>(cp * cy);
		value[4] = static_cast<float>(cp * sy);
		value[8] = static_cast<float>(-sp);
		const double srsp = sr * sp;
		const double crsp = cr * sp;

		value[1] = static_cast<float>(srsp * cy - cr * sy);
		value[5] = static_cast<float>(srsp * sy + cr * cy);
		value[9] = static_cast<float>(sr * cp);

		value[2] = static_cast<float>(crsp * cy + sr * sy);
		value[6] = static_cast<float>(crsp * sy - sr * cy);
		value[10] = static_cast<float>(cr * cp);
	}

	Matrix Matrix::transpose() const
	{
		Matrix result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.value[i * 4 + j] = value[j * 4 + i];
			}
		}
		return result;
	}

	Matrix Matrix::inverse() const
	{
		Matrix result(1);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.value[i * 4 + j] = value[j * 4 + i];
			}
		}
		result.value[12] = -value[12];
		result.value[13] = -value[13];
		result.value[14] = -value[14];
		return result;
	}

	Matrix& Matrix::operator=(const Matrix& mat)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				value[i * 4 + j] = mat.value[i * 4 + j];
			}
		}
		return *this;
	}

	Matrix Matrix::operator+(const Matrix& mat)
	{
		Matrix result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.value[i * 4 + j] = value[i * 4 + j] + mat.value[i * 4 + j];
			}
		}
		return result;
	}

	Matrix Matrix::operator-(const Matrix& mat)
	{
		Matrix result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.value[i * 4 + j] = value[i * 4 + j] - mat.value[i * 4 + j];
			}
		}
		return result;
	}

	Matrix Matrix::operator*(const Matrix& mat)
	{
		Matrix result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.value[i * 4 + j] = value[i * 4 + 0] * mat.value[0 * 4 + j] + value[i * 4 + 1] * mat.value[1 * 4 + j]
					+ value[i * 4 + 2] * mat.value[2 * 4 + j] + value[i * 4 + 3] * mat.value[3 * 4 + j];
			}
		}
		return result;
	}

	Matrix Matrix::operator*(const float& factor)
	{
		Matrix result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.value[i * 4 + j] = value[i * 4 + 0] * factor;
			}
		}
		return result;
	}

	bool Matrix::operator==(const Matrix& mat)
	{
		for (int i = 0; i < 16; i++)
		{
			if (value[i] != mat.value[i])
			{
				return false;
			}
		}
		return true;
	}

	bool Matrix::operator!=(const Matrix& mat)
	{
		for (int i = 0; i < 16; i++)
		{
			if (value[i] != mat.value[i])
			{
				return true;
			}
		}
		return true;
	}
}