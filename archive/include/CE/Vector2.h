#ifndef CE_VECTOR2_H
#define CE_VECTOR2_H

//- Standard Library 
#include <cassert>
#include <math.h>

namespace ce
{
	/* 	2d Vector Class
	 * 	- Data structure holding a 2d Vector.
	 */
	template<class Type> class Vector2
	{
		Type m_data[2];

	public:
		Vector2()
		{
			for(unsigned int a = 0; a < 2; a++)
				m_data[a] = (Type)0;
		}
		Vector2(Type x, Type y)
		{
			m_data[0] = x;
			m_data[1] = y;
		}
		Vector2(const Vector2 &t)
		{
			for(unsigned int a = 0; a < 2; a++)
				m_data[a] = t[a];
		}
		inline Type dot(const Vector2 &v) const
		{
			return m_data[0] * v[0] + m_data[1] * v[1];
		}
		inline Type getLength() const
		{
			return (Type)sqrt((double)(m_data[0] * m_data[0] + m_data[1] * m_data[1]));
		}
		inline Type getX() const
		{
			return m_data[0];
		}
		inline Type getY() const
		{
			return m_data[1];
		}
		inline void setX(Type x)
		{
			m_data[0] = x;
		}
		inline void setY(Type y)
		{
			m_data[1] = y;
		}
		inline Type &operator[](int i)
		{
			assert(i >= 0 && i < 2);
			return m_data[i];
		}
		inline Type operator[](int i) const
		{
			assert(i >= 0 && i < 2);
			return m_data[i];
		}
		inline Vector2 operator+(const Vector2 &v) const
		{
			return Vector2(m_data[0] + v[0], m_data[1] + v[1]);
		}
		inline Vector2 operator-(const Vector2 &v) const
		{
			return Vector2(m_data[0] - v[0], m_data[1] - v[1]);
		}
		inline Vector2 operator*(const Vector2 &v) const
		{
			return Vector2(m_data[0] * v[0], m_data[1] * v[1]);
		}
		inline Vector2 operator/(const Vector2 &v) const
		{
			return Vector2(m_data[0] / v[0], m_data[1] / v[1]);
		}
		inline Vector2 operator*(const Type t) const
		{
			return Vector2(m_data[0] * t, m_data[1] * t);
		}
		inline Vector2 operator/(const Type t) const
		{
			return Vector2(m_data[0] / t, m_data[1] / t);
		}
		inline Vector2 &operator+=(const Vector2 &v)
		{
			for(unsigned int a = 0; a < 2; a++)
				m_data[a] += v[a];
			return *this;
		}
		inline Vector2 &operator-=(const Vector2 &v)
		{
			for(unsigned int a = 0; a < 2; a++)
				m_data[a] -= v[a];
			return *this;
		}
		inline Vector2 &operator*=(const Vector2 &v)
		{
			for(unsigned int a = 0; a < 2; a++)
				m_data[a] *= v[a];
			return *this;
		}
		inline Vector2 &operator*=(const Type t)
		{
			for(unsigned int a = 0; a < 2; a++)
				m_data[a] *= t;
			return *this;
		}
		inline Vector2 &operator/=(const Type t)
		{
			for(unsigned int a = 0; a < 2; a++)
				m_data[a] /= t;
			return *this;
		}
		inline bool operator==(const Vector2 &v) const
		{
			return m_data[0] == v[0] && m_data[1] == v[1];
		}
		inline bool operator!=(const Vector2 &v) const
		{
			return m_data[0] != v[0] || m_data[1] != v[1];
		}
	};
}

#endif
