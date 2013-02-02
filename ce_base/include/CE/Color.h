#ifndef _CE_COLOR_H_
#define _CE_COLOR_H_

//- Standard Library -
#include <cassert>

namespace ce
{
	/* 	Color Class
	 * 	- Data structure holding color values in the form of unsigned bytes.
	 */
	class Color
	{
		unsigned char m_data[4];

	public:
		Color()
		{
			for(unsigned int a = 0; a < 4; a++)
				m_data[a] = 255;
		}
		Color(const Color &t)
		{
			for(unsigned int a = 0; a < 4; a++)
				m_data[a] = t[a];
		}
		Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
		{
			m_data[0] = red;
			m_data[1] = green;
			m_data[2] = blue;
			m_data[3] = alpha;
		}
		Color(const unsigned char *array)
		{
			for(unsigned int a = 0; a < 4; a++)
				m_data[a] = array[a];
		}
		inline unsigned char getAlpha() const
		{
			return m_data[3];
		}
		inline unsigned char getBlue() const
		{
			return m_data[2];
		}
		inline unsigned char getGreen() const
		{
			return m_data[1];
		}
		inline unsigned char getRed() const
		{
			return m_data[0];
		}
		inline void setAlpha(unsigned char alpha)
		{
			m_data[3] = alpha;
		}
		inline void setBlue(unsigned char blue)
		{
			m_data[2] = blue;
		}
		inline void setGreen(unsigned char green)
		{
			m_data[1] = green;
		}
		inline void setRed(unsigned char red)
		{
			m_data[0] = red;
		}
		inline unsigned char &operator[](int i)
		{
			assert(i >= 0 && i < 4);
			return m_data[i];
		}
		inline unsigned char const &operator[](int i) const
		{
			assert(i >= 0 && i < 4);
			return m_data[i];
		}
		inline bool operator==(const Color &v) const
		{
			return m_data[0] == v[0] && m_data[1] == v[1] && m_data[2] == v[2] && m_data[3] == v[3];
		}
		inline bool operator!=(const Color &v) const
		{
			return m_data[0] != v[0] || m_data[1] != v[1] || m_data[2] != v[2] || m_data[3] != v[3];
		}
	};
}

#endif
