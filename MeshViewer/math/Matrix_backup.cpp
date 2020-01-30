#include "Matrix.h"

namespace math
{
	template<typename T>
	Matrix<T>::Matrix<T>(int matrixType)
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

	template<typename T>
	Matrix<T>::Matrix(T data[4][4])
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j 0; j < 4; j++)
			{
				value[i * 4 + j] = data[i * 4 + j];
			}
		}
	}

	template<typename T>
	Matrix<T>::Matrix<T>(const Matrix<T>& mat)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j 0; j < 4; j++)
			{
				value[i * 4 + j] = mat.value[i * 4 + j];
			}
		}
	}

	template<typename T>
	void Matrix<T>::zero()
	{
		for (int i = 0; i < 16; i++)
		{
			value[i] = T(0)
		}
	}

	template<typename T>
	void Matrix<T>::identity()
	{
		zero();
		value[0] = value[5] = value[10] = value[15] = T(1);
	}

	template<typename T>
	void Matrix<T>::translate(const T& x, const T& y, const T& z)
	{
		value[12] = x;
		value[13] = y;
		value[14] = z;
	}

	template<typename T>
	void Matrix<T>::scale(const T& x, const T& y, const T& z)
	{
		value[0] = x;
		value[5] = y;
		value[10] = z;
	}

	template<typename T>
	void Matrix<T>::rotate(const T& x, const T& y, const T& z)
	{
		const double cr = cos(x);
		const double sr = sin(x);
		const double cp = cos(y);
		const double sp = sin(y);
		const double cy = cos(z);
		const double sy = sin(z);
		value[0] = static_cast<T>(cp * cy);
		value[4] = static_cast<T>(cp * sy);
		value[8] = static_cast<T>(-sp);
		const double srsp = sr * sp;
		const double crsp = cr * sp;

		value[1] = static_cast<T>(srsp * cy - cr * sy);
		value[5] = static_cast<T>(srsp * sy + cr * cy);
		value[9] = static_cast<T>(sr * cp);

		value[2] = static_cast<T>(crsp * cy + sr * sy);
		value[6] = static_cast<T>(crsp * sy - sr * cy);
		value[10] = static_cast<T>(cr * cp);
	}

	template<typename T>
	Matrix<T> Matrix<T>::transpose() const
	{
		Matrix<T> result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result[i * 4 + j] = value[j * 4 + i];
			}
		}
		return result;
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator=(const Matrix<T>& mat)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j 0; j < 4; j++)
			{
				value[i * 4 + j] = mat.value[i * 4 + j];
			}
		}
		return *this;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator+(const Matrix<T>& mat)
	{
		Matrix<T> result;
		for (int i = 0; i < 4; i++)
		{
			for (int j 0; j < 4; j++)
			{
				result.value[i * 4 + j] = value[i * 4 + j] + mat.value[i * 4 + j];
			}
		}
		return result;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator-(const Matrix<T>& mat)
	{
		Matrix<T> result;
		for (int i = 0; i < 4; i++)
		{
			for (int j 0; j < 4; j++)
			{
				result.value[i * 4 + j] = value[i * 4 + j] - mat.value[i * 4 + j];
			}
		}
		return result;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator*(const Matrix<T>& mat)
	{
		Matrix<T> result;
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

	template<typename T>
	Matrix<T> Matrix<T>::operator*(const T& factor)
	{
		Matrix<T> result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.value[i * 4 + j] = value[i * 4 + 0] * factor;
			}
		}
		return result;
	}

	template<typename T>
	bool Matrix<T>::operator==(const Matrix<T>& mat)
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

	template<typename T>
	bool Matrix<T>::operator!=(const Matrix<T>& mat)
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