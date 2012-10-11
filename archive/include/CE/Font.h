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
		std::map<unsigned int, void *> m_sdlFontMap;
		void *m_sdlRWops;

		Font();

	public:
		static Font *createFromFile(const char *file);
		static Font *createFromMemory(const unsigned char *memory, unsigned int size);
		static Font *getDefault();
		static void deleteDefault();

		~Font();

		void drawText(const char *text, unsigned int size);
		void drawUnicodeChar(unsigned short unicodeChar, unsigned int size);
	};
}

#endif