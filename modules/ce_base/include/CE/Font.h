#ifndef CE_FONT_H
#define CE_FONT_H

//- Standard Library -
#include <map>

namespace ce
{
	/* 	Font Class
	 * 	- Acts as specialized handle for different fonts.
	 */
	class Font
	{
		static Font *ms_default;

		std::map<unsigned int, std::map<unsigned short, unsigned int> > m_glDisplayListMap;
		std::map<unsigned int, std::map<unsigned short, unsigned int> > m_glTextureMap;
		void *m_ftFace;
		unsigned int m_height;

		Font();

	public:
		static bool Init();
		static Font *CreateFromFile(const char *file);
		static Font *CreateFromMemory(const unsigned char *memory, unsigned int size);

		static Font *GetDefault();
		static void DeleteDefault();

		~Font();

		void DrawText(const char *text);
		void DrawUnicodeChar(unsigned short unicodeChar);
		void SetCharSize(unsigned int width, unsigned int height, unsigned int horizontalDpi, unsigned int verticalDpi);
	};
}

#endif