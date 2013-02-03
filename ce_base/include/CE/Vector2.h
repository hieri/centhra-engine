#ifndef _CE_VECTOR2_H_
#define _CE_VECTOR2_H_

//- Standard Library 
#include <cassert>
#include <math.h>

namespace ce
{
	/**	@brief 2d Vector Data Structure Class
	 */
	template<class Type> class Vector2
	{
		Type m_data[2];

	public:
		/**	@brief Default Constructor.
		 */
		Vector2()
		{
			for(unsigned int a = 0; a < 2; a++)
				m_data[a] = (Type)0;
		}

		/**	@brief Templated Constructor.
		 */
		Vector2(Type x, Type y)
		{
			m_data[0] = x;
			m_data[1] = y;
		}

		/**	@brief Returns the dot product of the vector with another.
		 */
		inline Type Dot(const Vector2 &v) const
		{
			return m_data[0] * v[0] + m_data[1] * v[1];
		}

		/**	@brief Returns the length of the vector.
		 */
		inline Type GetLength() const
		{
			return (Type)sqrt((double)(m_data[0] * m_data[0] + m_data[1] * m_data[1]));
		}

		/**	@brief Returns the x component of the vector.
		 */
		inline Type GetX() const
		{
			return m_data[0];
		}

		/**	@brief Returns the y component of the vector.
		 */
		inline Type GetY() const
		{
			return m_data[1];
		}

		/**	@brief Sets the x component of the vector.
		 */
		inline void SetX(Type x)
		{
			m_data[0] = x;
		}

		/**	@brief Sets the y component of the vector.
		 */
		inline void SetY(Type y)
		{
			m_data[1] = y;
		}

		/**	@brief Arrayed accessor operator.
		 */
		inline Type &operator[](int i)
		{
			assert(i >= 0 && i < 2);
			return m_data[i];
		}

		/**	@brief Constant arrayed accessor operator.
		 */
		inline Type const &operator[](int i) const
		{
			assert(i >= 0 && i < 2);
			return m_data[i];
		}

		/**	@brief Addition operator.
		 */
		inline Vector2 operator+(const Vector2 &v) const
		{
			return Vector2(m_data[0] + v[0], m_data[1] + v[1]);
		}

		/**	@brief Subtraction operator.
		 */
		inline Vector2 operator-(const Vector2 &v) const
		{
			return Vector2(m_data[0] - v[0], m_data[1] - v[1]);
		}

		/**	@brief Multiplication operator.
		 */
		inline Vector2 operator*(const Vector2 &v) const
		{
			return Vector2(m_data[0] * v[0], m_data[1] * v[1]);
		}

		/**	@brief Scalar multiplication operator.
		 */
		inline Vector2 operator*(const Type t) const
		{
			return Vector2(m_data[0] * t, m_data[1] * t);
		}

		/**	@brief Scalar division operator.
		 */
		inline Vector2 operator/(const Type t) const
		{
			return Vector2(m_data[0] / t, m_data[1] / t);
		}

		/**	@brief Additive operator.
		 */
		inline Vector2 &operator+=(const Vector2 &v)
		{
			for(unsigned int a = 0; a < 2; a++)
				m_data[a] += v[a];
			return *this;
		}

		/**	@brief Subtractive operator.
		 */
		inline Vector2 &operator-=(const Vector2 &v)
		{
			for(unsigned int a = 0; a < 2; a++)
				m_data[a] -= v[a];
			return *this;
		}

		/**	@brief Multiplicative operator.
		 */
		inline Vector2 &operator*=(const Vector2 &v)
		{
			for(unsigned int a = 0; a < 2; a++)
				m_data[a] *= v[a];
			return *this;
		}

		/**	@brief Scalar multiplicative operator.
		 */
		inline Vector2 &operator*=(const Type t)
		{
			for(unsigned int a = 0; a < 2; a++)
				m_data[a] *= t;
			return *this;
		}

		/**	@brief Scalar divisive operator.
		 */
		inline Vector2 &operator/=(const Type t)
		{
			for(unsigned int a = 0; a < 2; a++)
				m_data[a] /= t;
			return *this;
		}

		/**	@brief Equals comparison operator.
		 */
		inline bool operator==(const Vector2 &v) const
		{
			return m_data[0] == v[0] && m_data[1] == v[1];
		}

		/**	@brief NotEquals comparison operator.
		 */
		inline bool operator!=(const Vector2 &v) const
		{
			return m_data[0] != v[0] || m_data[1] != v[1];
		}
	};
}

#endif
