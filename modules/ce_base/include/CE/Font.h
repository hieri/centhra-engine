#ifndef _CE_BASE_FONT_H_
#define _CE_BASE_FONT_H_
#if CE_CLIENT 

//- Centhra Engine -
#include <CE/Matrix4x4.h>
#include <CE/RenderContext.h>
#include <CE/Vector2.h>

//- Standard Library -
#include <map>

namespace ce
{
	/**	@brief Font Data Structure Class
	 */
	class Font
	{
		static Font *ms_default;

		typedef struct DisplayInfo
		{
			Matrix4x4<float> advanceMatrix;
			unsigned int offset;
			int_canvas bearingX, bearingY, width, height, advance;
		} DisplayInfo;

		std::map<unsigned short, DisplayInfo> m_displayInfoMap;

		void *m_ftFace;
		float m_horizontalDpi, m_verticalDpi;
		unsigned int m_glBuffer, m_glTexture;

		Font(void *face, unsigned int charSize, float horizontalDpi, float verticalDpi);

		void DeleteDisplayData();
		void GenerateDisplayData();

	public:
		/**	@brief Initializes the registered font loaders.
		 *	@return Whether or not the font loader initialization succeeded.
		 */
		static bool Init();

		/**	@brief File Constructor.
		 *	@param file Path to font file.
		 *	@return Font object or 0 if creation failed.
		 */
		static Font *CreateFromFile(const char *file, unsigned int charSize, float horizontalDpi, float verticalDpi);

		/**	@brief Memory Constructor.
		 *	@param memory Address of font data.
		 *	@param size Size of font data.
		 *	@return Font object or 0 if creation failed.
		 */
		static Font *CreateFromMemory(const unsigned char *memory, unsigned int size, unsigned int charSize, float horizontalDpi, float verticalDpi);

		/**	@brief Destructor.
		 */
		~Font();

		/**	@brief Draws a string.
		 *	@param str The string that will be drawn.
		 */
		void DrawString(RenderContext &context, const char *str, unsigned int newLineAdvance = 4);
		void DrawStringUI(RenderContext &context, const char *str, unsigned int newLineAdvance = 4);

		/**	@brief Draws a unicode character.
		 *	@param unicodeChar The unicode character that will be drawn.
		 */
		void DrawUnicodeChar(RenderContext &context, unsigned short unicodeChar);
		void DrawUnicodeCharUI(RenderContext &context, unsigned short unicodeChar);

		void SetDPI(float horizontalDpi, float verticalDpi);

		Vector2<int> StringDimensions(const char *str, unsigned int newLineAdvance = 4);

		//------------ Character Size ------------
	protected:
		unsigned int m_charSize;
	public:
		/**	@brief Sets the character size of the Font.
		*	@param size The size of a character.
		*/
		void SetCharacterSize(unsigned int size);

		/**	@brief Returns the character size of the font.
		*	@return The character size of the Font.
		*/
		unsigned int GetCharSize() const;
	};
}

#endif
#endif
