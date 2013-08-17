#ifndef _CE_BASE_COLOR_H_
#define _CE_BASE_COLOR_H_

//- Standard Library -
#include <cassert>

namespace ce
{
	/**	@brief Color Data Structure Class
	 *	
	 *	Data structure holding color values in the form of unsigned bytes.
	 */
	class Color
	{
		unsigned char m_data[4];

	public:
		/**	@brief Default Constructor.
		 *	@param red Red component value.
		 *	@param green Green component value.
		 *	@param blue Blue component value.
		 *	@param alpha Alpha component value.
		 */
		Color(unsigned char red = 255, unsigned char green = 255, unsigned char blue = 255, unsigned char alpha = 255) 
		{
			m_data[0] = red;
			m_data[1] = green;
			m_data[2] = blue;
			m_data[3] = alpha;
		}

		/**	@brief Array Constructor.
		 *	@param array Arrayed color data.
		 */
		Color(const unsigned char *array)
		{
			for(unsigned int a = 0; a < 4; a++)
				m_data[a] = array[a];
		}

		/**	@brief Returns the alpha component of the color.
		 *	@return Alpha component of the color.
		 */
		inline unsigned char GetAlpha() const
		{
			return m_data[3];
		}

		/**	@brief Returns the blue component of the color.
		 *	@return Blue component of the color.
		 */
		inline unsigned char GetBlue() const
		{
			return m_data[2];
		}

		/**	@brief Returns the green component of the color.
		 *	@return Green component of the color.
		 */
		inline unsigned char GetGreen() const
		{
			return m_data[1];
		}

		/**	@brief Returns the red component of the color.
		 *	@return Red component of the color.
		 */
		inline unsigned char GetRed() const
		{
			return m_data[0];
		}

		/**	@brief Sets the alpha component of the color.
		 *	@param alpha Alpha component value.
		 */
		inline void SetAlpha(unsigned char alpha)
		{
			m_data[3] = alpha;
		}

		/**	@brief Sets the blue component of the color.
		 *	@param blue Blue component value.
		 */
		inline void SetBlue(unsigned char blue)
		{
			m_data[2] = blue;
		}

		/**	@brief Sets the green component of the color.
		 *	@param green Green component value.
		 */
		inline void SetGreen(unsigned char green)
		{
			m_data[1] = green;
		}

		/**	@brief Set the red component of the color.
		 *	@param red Red component value.
		 */
		inline void SetRed(unsigned char red)
		{
			m_data[0] = red;
		}

		/**	@brief Arrayed accessor operator.
		 */
		inline unsigned char &operator[](int i)
		{
			assert(i >= 0 && i < 4);
			return m_data[i];
		}

		/**	@brief Constant arrayed accessor operator.
		 */
		inline unsigned char const &operator[](int i) const
		{
			assert(i >= 0 && i < 4);
			return m_data[i];
		}

		/**	@brief Equals comparison operator.
		 */
		inline bool operator==(const Color &v) const
		{
			return m_data[0] == v[0] && m_data[1] == v[1] && m_data[2] == v[2] && m_data[3] == v[3];
		}

		/**	@brief NotEquals comparison operator.
		 */
		inline bool operator!=(const Color &v) const
		{
			return m_data[0] != v[0] || m_data[1] != v[1] || m_data[2] != v[2] || m_data[3] != v[3];
		}
	};
}

#endif
