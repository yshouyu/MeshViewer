#include "Vector.h"

namespace math
{
	template<typename T>
	Vector<T>::Vector<T>(T vx /* = T(0) */, T vy /* = T(0) */, T vz /* = T(0) */, T vw /* = T(0) */)
		: x(vx), y(vy), z(vz), w(vw)
	{

	}

	template<typename T>
	Vector<T>::Vector<T>(const Vector& v)
		: x(v.x), y(v.y), z(v.z), w(v.w)
	{

	}

	template<typename T>
	void Vector<T>::set(T vx /* = T(0) */, T vy /* = T(0) */, T vz /* = T(0) */, T vw /* = T(0) */)
	{
		x = vx;
		y = vy;
		z = vz;
		w = vw;
	}

	template<typename T>
	T Vector<T>::length()
	{
		return sqrt(x * x + y * y + z * z);
	}

	template<typename T>
	T Vector<T>::lengthsq()
	{
		return x * x + y * y + z * z;
	}

	template<typename T>
	void Vector<T>::zero()
	{
		x = y = z = T(0);
		w = T(1);
	}

	template<typename T>
	T Vector<T>::dot(const Vector<T>& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	template<typename T>
	Vector<T>& Vector<T>::cross(const Vector<T>& v)
	{
		T tempx = y * v.z - z * v.y;
		T tempy = z * v.x - x * v.z;
		T tempz = x * v.y - y * v.x;
		x = tempx;
		y = tempy;
		z = tempz;
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::normalize()
	{
		T length = length();
		if (length > T(0))
		{
			T factor = T(1) / length;
			x *= factor;
			y *= factor;
			z *= factor;
		}
		else
		{
			return *this;
		}
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(const Vector<T>& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	template<typename T>
	T& Vector<T>::operator[](int i)
	{
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			break;
		}
	}

	template<typename T>
	Vector<T> Vector<T>::operator+(const Vector<T>& v) const
	{
		return Vector<T>(v + v.x, y + v.y, z + v.z, 1);
	}

	template<typename T>
	Vector<T> Vector<T>::operator+(const T& v) const
	{
		return Vector<T>(v + v, y + v, z + v, 1);
	}

	template<typename T>
	Vector<T>& Vector<T>::operator+=(const Vector<T>& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w = 1;
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator+=(const T& v)
	{
		x += v;
		y += v;
		z += v;
		w = 1;
		return *this;
	}

	template<typename T>
	Vector<T> Vector<T>::operator-(const Vector<T>& v) const
	{
		return Vector<T>(v - v.x, y - v.y, z - v.z, 1);
	}

	template<typename T>
	Vector<T> Vector<T>::operator-(const T& v) const
	{
		return Vector<T>(v - v, y - v, z - v, 1);
	}

	template<typename T>
	Vector<T>& Vector<T>::operator-=(const Vector<T>& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w = 1;
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator-=(const T& v)
	{
		x -= v;
		y -= v;
		z -= v;
		w = 1;
		return *this;
	}

	template<typename T>
	Vector<T> Vector<T>::operator*(const Vector<T>& v) const
	{
		return Vector<T>(v * v.x, y * v.y, z * v.z, 1);
	}

	template<typename T>
	Vector<T> Vector<T>::operator*(const T& v) const
	{
		return Vector<T>(v * v, y * v, z * v, 1);
	}

	template<typename T>
	Vector<T>& Vector<T>::operator*=(const Vector<T>& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w = 1;
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator*=(const T& v)
	{
		x *= v;
		y *= v;
		z *= v;
		w = 1;
		return *this;
	}

	template<typename T>
	Vector<T> Vector<T>::operator/(const Vector<T>& v) const
	{
		return Vector<T>(v / v.x, y / v.y, z / v.z, 1);
	}

	template<typename T>
	Vector<T> Vector<T>::operator/(const T& v) const
	{
		return Vector<T>(v / v, y / v, z / v, 1);
	}

	template<typename T>
	Vector<T>& Vector<T>::operator/=(const Vector<T>& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w = 1;
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator/=(const T& v)
	{
		x /= v;
		y /= v;
		z /= v;
		w = 1;
		return *this;
	}

	template<typename T>
	bool Vector<T>::operator==(const Vector& v) const
	{
		return x == v.x && y == v.y && z == v.z && w == v.w;
	}

	template<typename T>
	bool Vector<T>::operator!=(const Vector& v) const
	{
		return x != v.x || y != v.y || z != v.z || w != v.w;
	}

	template<typename T>
	T Vector<T>::getDot(const Vector<T>& v1, const Vector<T>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	template<typename T>
	Vector<T> Vector<T>::getCross(const Vector<T>& v1, const Vector<T>& v2)
	{
		return Vector<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}
}