#ifndef CE_RECT_H
#define CE_RECT_H

//- Standard Library 
#include <cassert>
#include <math.h>

namespace ce
{
	/* 	Rectangle Class
	 * 	- Data structure holding a rectangle.
	 */
	template<class Type> class Rect
	{
		Type m_data[4];

	public:
		Rect()
		{
			for(unsigned int a = 0; a < 4; a++)
				m_data[a] = (Type)0;
		}
		Rect(Type left, Type top, Type right, Type bottom)
		{
			m_data[0] = left;
			m_data[1] = top;
			m_data[2] = right;
			m_data[3] = bottom;
		}
		Rect(const Rect &t)
		{
			for(unsigned int a = 0; a < 4; a++)
				m_data[a] = t[a];
		}
		inline Type getBottom() const
		{
			return m_data[3];
		}
		inline Type getHeight() const
		{
			return m_data[3] - m_data[1];
		}
		inline Type getLeft() const
		{
			return m_data[0];
		}
		inline Type getRight() const
		{
			return m_data[2];
		}
		inline Type getTop() const
		{
			return m_data[1];
		}
		inline Type getWidth() const
		{
			return m_data[2] - m_data[0];
		}
		inline void setBottom(Type bottom)
		{
			m_data[3] = bottom;
		}
		inline void setLeft(Type left)
		{
			m_data[0] = left;
		}
		inline void setRight(Type right)
		{
			m_data[2] = right;
		}
		inline void setTop(Type top)
		{
			m_data[1] = top;
		}
		inline Type &operator[](int i)
		{
			assert(i >= 0 && i < 4);
			return m_data[i];
		}
		inline Type operator[](int i) const
		{
			assert(i >= 0 && i < 4);
			return m_data[i];
		}
		inline bool operator==(const Rect &v) const
		{
			return m_data[0] == v[0] && m_data[1] == v[1] && m_data[2] == v[2] && m_data[3] == v[3];
		}
		inline bool operator!=(const Rect &v) const
		{
			return m_data[0] != v[0] || m_data[1] != v[1] || m_data[2] != v[2] || m_data[3] != v[3];
		}
	};
}

#endif
