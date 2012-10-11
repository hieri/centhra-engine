#ifndef CE_VECTOR3_H
#define CE_VECTOR3_H

//- Standard Library 
#include <cassert>
#include <math.h>

namespace ce
{
	/* 	3d Vector Class
	 * 	- Data structure holding a 3d Vector.
	 */
	template<class Type> class Vector3
	{
		Type m_data[3];
	public:
		Vector3()
		{
			for(unsigned int a = 0; a < 3; a++)
				m_data[a] = (Type)0;
		}
		Vector3(Type x, Type y, Type z)
		{
			m_data[0] = x;
			m_data[1] = y;
			m_data[2] = z;
		}
		Vector3(const Vector3 &t)
		{
			for(unsigned int a = 0; a < 3; a++)
				m_data[a] = t[a];
		}
		inline Vector3 cross(const Vector3 &v) const
		{
			return Vector3(m_data[1] * v[2] - m_data[2] * v[1], m_data[2] * v[0] - m_data[0] * v[2], m_data[0] * v[1] - m_data[1] * v[0]);
		}
		inline Type dot(const Vector3 &v) const
		{
			return m_data[0] * v[0] + m_data[1] * v[1] + m_data[2] * v[2];
		}
		inline Type getLength() const
		{
			return (Type)sqrt((double)(m_data[0] * m_data[0] + m_data[1] * m_data[1] + m_data[2] * m_data[2]));
		}
		inline Type getX() const
		{
			return m_data[0];
		}
		inline Type getY() const
		{
			return m_data[1];
		}
		inline Type getZ() const
		{
			return m_data[2];
		}
		inline Vector3 &normalize()
		{
			Type length = getLength();
			if(length)
				*this /= length;
			return *this;
		}
		inline void setX(Type x)
		{
			m_data[0] = x;
		}
		inline void setY(Type y)
		{
			m_data[1] = y;
		}
		inline void setZ(Type z)
		{
			m_data[2] = z;
		}
		inline Type &operator[](int i)
		{
			assert(i >= 0 && i < 3);
			return m_data[i];
		}
		inline Type operator[](int i) const
		{
			assert(i >= 0 && i < 3);
			return m_data[i];
		}
		inline Vector3 operator+(const Vector3 &v) const
		{
			return Vector3(m_data[0] + v[0], m_data[1] + v[1], m_data[2] + v[2]);
		}
		inline Vector3 operator-(const Vector3 &v) const
		{
			return Vector3(m_data[0] - v[0], m_data[1] - v[1], m_data[2] - v[2]);
		}
		inline Vector3 operator*(const Vector3 &v) const
		{
			return Vector3(m_data[0] * v[0], m_data[1] * v[1], m_data[2] * v[2]);
		}
		inline Vector3 operator/(const Vector3 &v) const
		{
			return Vector3(m_data[0] / v[0], m_data[1] / v[1], m_data[2] / v[2]);
		}
		inline Vector3 operator*(const Type t) const
		{
			return Vector3(m_data[0] * t, m_data[1] * t, m_data[2] * t);
		}
		inline Vector3 operator/(const Type t) const
		{
			return Vector3(m_data[0] / t, m_data[1] / t, m_data[2] / t);
		}
		inline Vector3 &operator+=(const Vector3 &v)
		{
			for(unsigned int a = 0; a < 3; a++)
				m_data[a] += v[a];
			return *this;
		}
		inline Vector3 &operator-=(const Vector3 &v)
		{
			for(unsigned int a = 0; a < 3; a++)
				m_data[a] -= v[a];
			return *this;
		}
		inline Vector3 &operator*=(const Vector3 &v)
		{
			for(unsigned int a = 0; a < 3; a++)
				m_data[a] *= v[a];
			return *this;
		}
		inline Vector3 &operator*=(const Type t)
		{
			for(unsigned int a = 0; a < 3; a++)
				m_data[a] *= t;
			return *this;
		}
		inline Vector3 &operator/=(const Type t)
		{
			for(unsigned int a = 0; a < 3; a++)
				m_data[a] /= t;
			return *this;
		}
		inline bool operator==(const Vector3 &v) const
		{
			return m_data[0] == v[0] && m_data[1] == v[1] && m_data[2] == v[2];
		}
		inline bool operator!=(const Vector3 &v) const
		{
			return m_data[0] != v[0] || m_data[1] != v[1] || m_data[2] != v[2];
		}
	};
}

#endif
