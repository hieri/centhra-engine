#ifndef _CE_BASE_RECT_H_
#define _CE_BASE_RECT_H_

//- Standard Library 
#include <cmath>
#include <cassert>

namespace ce
{
	template<class Type> class Rect
	{
		Type m_data[4];

	public:
		Rect()
		{
			for(unsigned int a = 0; a < 4; a++)
				m_data[a] = 0;
		}
		Rect(Type minX, Type minY, Type maxX, Type maxY)
		{
			m_data[0] = minX;
			m_data[1] = minY;
			m_data[2] = maxX;
			m_data[3] = maxY;
		}
		Rect(const Rect &t)
		{
			for(unsigned int a = 0; a < 4; a++)
				m_data[a] = t[a];
		}
		inline Type GetMinY() const
		{
			return m_data[1];
		}
		inline Type GetMaxY() const
		{
			return m_data[3];
		}
		inline Type GetHeight() const
		{
			return m_data[3] - m_data[1];
		}
		inline Type GetMinX() const
		{
			return m_data[0];
		}
		inline Type GetMaxX() const
		{
			return m_data[2];
		}
		inline Type GetWidth() const
		{
			return m_data[2] - m_data[0];
		}
		inline void SetMinY(Type minY)
		{
			m_data[1] = minY;
		}
		inline void SetMaxY(Type maxY)
		{
			m_data[3] = maxY;
		}
		inline void SetMinX(Type minX)
		{
			m_data[0] = minX;
		}
		inline void SetMaxX(Type maxX)
		{
			m_data[2] = maxX;
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
