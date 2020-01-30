#ifndef __VECTOR_H__
#define __VECTOR_H__

namespace math
{
	template <typename T>
	class Vector 
	{
	public:
		explicit Vector(T vx = T(0), T vy = T(0), T vz = T(0), T vw = T(0));
		Vector<T>(const Vector<T>& v);
		void set(T vx = T(0), T vy = T(0), T vz = T(0), T vw = T(0));
		T length();
		T lengthsq();
		void zero();
		Vector<T>& normalize();
		T dot(const Vector<T>& v) const;
		static T getDot(const Vector<T>& v1, const Vector<T>& v2);
		Vector<T>& cross(const Vector<T>& v);
		static Vector<T> getCross(const Vector<T>& v1, const Vector<T>& v2);
		Vector<T>& operator=(const Vector<T>& v);
		T& operator[](int i);
		Vector<T> operator+(const Vector<T>& v) const;
		Vector<T> operator+(const T& v) const;
		Vector<T>& operator+=(const Vector<T>& v);
		Vector<T>& operator+=(const T& v);
		Vector<T> operator-(const Vector<T>& v) const;
		Vector<T> operator-(const T& v) const;
		Vector<T>& operator-=(const Vector<T>& v);
		Vector<T>& operator-=(const T& v);
		Vector<T> operator*(const Vector<T>& v) const;
		Vector<T> operator*(const T& v) const;
		Vector<T>& operator*=(const Vector<T>& v);
		Vector<T>& operator*=(const T& v);
		Vector<T> operator/(const Vector<T>& v) const;
		Vector<T> operator/(const T& v) const;
		Vector<T>& operator/=(const Vector<T>& v);
		Vector<T>& operator/=(const T& v);
		bool operator==(const Vector<T>& v) const;
		bool operator!=(const Vector<T>& v) const;
	public :
		T x, y, z, w;
	};
}

#endif