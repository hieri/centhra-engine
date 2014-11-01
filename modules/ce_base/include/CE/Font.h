#ifndef _CE_BASE_FONT_H_
#define _CE_BASE_FONT_H_

//- Standard Library -
#include <map>

namespace ce
{
	/**	@brief Font Data Structure Class
	 */
	class Font
	{
		static Font *ms_default;

		std::map<unsigned int, std::map<unsigned short, unsigned int> > m_glTextureMap, m_glDisplayListMap, m_glUIDisplayListMap;
		void *m_ftFace;
		unsigned int m_height;

		Font();

	public:
		/**	@brief Initializes the registered font loaders.
		 *	@return Whether or not the font loader initialization succeeded.
		 */
		static bool Init();

		/**	@brief File Constructor.
		 *	@param file Path to font file.
		 *	@return Font object or 0 if creation failed.
		 */
		static Font *CreateFromFile(const char *file);

		/**	@brief Memory Constructor.
		 *	@param memory Address of font data.
		 *	@param size Size of font data.
		 *	@return Font object or 0 if creation failed.
		 */
		static Font *CreateFromMemory(const unsigned char *memory, unsigned int size);

		/**	@brief Destructor.
		 */
		~Font();

		/**	@brief Draws a string.
		 *	@param str The string that will be drawn.
		 */
		void DrawString(const char *str);
		void DrawStringUI(const char *str);

		/**	@brief Draws a unicode character.
		 *	@param unicodeChar The unicode character that will be drawn.
		 */
		void DrawUnicodeChar(unsigned short unicodeChar);
		void DrawUnicodeCharUI(unsigned short unicodeChar);

		/**	@brief Sets the character size of the Font.
		 *	@param width The width of a character.
		 *	@param height The height of a character.
		 *	@param horizontalDpi The horizontal dpi of a character.
		 *	@param verticalDpi The vertical dpi of a character.
		 */
		void SetCharSize(unsigned int width, unsigned int height, unsigned int horizontalDpi, unsigned int verticalDpi);

		/**	@brief Returns the character height of the font.
		 *	@return The character height of the Font.
		 */
		unsigned int GetCharHeight() const;
	};
}

#endif
