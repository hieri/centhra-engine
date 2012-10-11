#ifdef _MSC_VER
	#include <windows.h>
#endif

//- SDL2 -
#include <SDL_opengl.h>
#include <SDL_rwops.h>
#include <SDL_ttf.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Font.h>
#include <CE/Resource_Base.h>

extern void *_binary_FreeMono_ttf_start, *_binary_FreeMono_ttf_size;

using namespace std;

// TODO: Integrate the ability to get Font metrices.
// TODO: Add UNICODE escape conversions.

namespace ce
{
	Font *Font::ms_default = 0;
	Font *Font::getDefault()
	{
		if(!ms_default)
		{
#ifdef _MSC_VER
			HRSRC infoFreeMonoFont = FindResource(0, MAKEINTRESOURCE(CE_FONT_FREEMONO), RT_FONT);
			HGLOBAL resFreeMonoFont = LoadResource(0, infoFreeMonoFont);

			unsigned int size = (unsigned int)SizeofResource(0, infoFreeMonoFont);
			ms_default = Font::createFromMemory((unsigned char *)LockResource(resFreeMonoFont), size);

			FreeResource(resFreeMonoFont);
#else
			ms_default = Font::createFromMemory((const unsigned char *)&_binary_FreeMono_ttf_start, (unsigned long)&_binary_FreeMono_ttf_size);
#endif
		}
		return ms_default;
	}
	Font *Font::createFromFile(const char *file)
	{
		if(!fileExists(file))
		{
			setError("Font::createFromFile: File does not exist <%s>.", file);
			return 0;
		}

		SDL_RWops *sdlRWops = SDL_RWFromFile(file, "rb");
		Font *font = new Font;
		font->m_sdlRWops = sdlRWops;
		return font;
	}
	Font *Font::createFromMemory(const unsigned char *memory, unsigned int size)
	{
		if(!memory)
		{
			setError("Image::createFromMemory: Empty memory pointer");
			return 0;
		}

		SDL_RWops *sdlRWops = SDL_RWFromConstMem(memory, size);
		Font *font = new Font;
		font->m_sdlRWops = sdlRWops;
		return font;
	}
	void Font::deleteDefault()
	{
		if(ms_default)
			delete ms_default;
		ms_default = 0;
	}
	Font::Font()
	{
	}
	Font::~Font()
	{
		for(map<unsigned int, map<unsigned short, unsigned int> >::iterator itA = m_glDisplayListMap.begin(); itA != m_glDisplayListMap.end(); itA++)
			for(map<unsigned short, unsigned int>::iterator itB = itA->second.begin(); itB != itA->second.end(); itB++)
				glDeleteLists(itB->second, 1);
		for(map<unsigned int, map<unsigned short, unsigned int> >::iterator itA = m_glTextureMap.begin(); itA != m_glTextureMap.end(); itA++)
			for(map<unsigned short, unsigned int>::iterator itB = itA->second.begin(); itB != itA->second.end(); itB++)
				glDeleteTextures(1, &itB->second);
		for(map<unsigned int, void *>::iterator it = m_sdlFontMap.begin(); it != m_sdlFontMap.end(); it++)
			TTF_CloseFont((TTF_Font *)it->second);
		if(m_sdlRWops)
			SDL_FreeRW((SDL_RWops *)m_sdlRWops);
	}
	void Font::drawText(const char *text, unsigned int size)
	{
		TTF_Font *font = 0;
		if(m_sdlFontMap.count(size))
			font = (TTF_Font *)m_sdlFontMap[size];
		else
		{
			font = TTF_OpenFontRW((SDL_RWops *)m_sdlRWops, 0, size);
			if(!font)
				setError("TTF_OpenFontRW: %s", TTF_GetError());
			m_sdlFontMap[size] = font;
		}

		if(font)
			for(unsigned int a = 0; a < strlen(text); a++)
				drawUnicodeChar(text[a], size);
	}
	void Font::drawUnicodeChar(unsigned short unicodeChar, unsigned int size)
	{
		unsigned int glDisplayList = 0;
		unsigned int glTexture = 0;
		if(m_glDisplayListMap.count(size))
			if(m_glDisplayListMap[size].count(unicodeChar))
			{
				glDisplayList = m_glDisplayListMap[size][unicodeChar];
				glTexture = m_glTextureMap[size][unicodeChar];
			}
		if(!glDisplayList)
		{
			SDL_Color white = {255, 255, 255};
			TTF_Font *font = (TTF_Font *)m_sdlFontMap[size];

			unsigned short text[] = {unicodeChar, 0};
			SDL_Surface *sdlSurface = TTF_RenderUNICODE_Blended(font, text, white);

			glGenTextures(1, &glTexture);

			glBindTexture(GL_TEXTURE_2D, glTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, 4, sdlSurface->w, sdlSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sdlSurface->pixels);

			int minX, maxX, minY, maxY, advance;
			TTF_GlyphMetrics(font, unicodeChar, &minX, &maxX, &minY, &maxY, &advance);

			glDisplayList = glGenLists(1);
			glNewList(glDisplayList, GL_COMPILE);
				glBegin(GL_QUADS);
					glTexCoord2i(0, 0); glVertex2i(0, 0);
					glTexCoord2i(1, 0); glVertex2i(sdlSurface->w, 0);
					glTexCoord2i(1, 1); glVertex2i(sdlSurface->w, sdlSurface->h);
					glTexCoord2i(0, 1); glVertex2i(0, sdlSurface->h);
				glEnd();
				glTranslatef((float)advance, 0.f, 0.f);
			glEndList();

			SDL_FreeSurface(sdlSurface);

			m_glDisplayListMap[size][unicodeChar] = glDisplayList;
			m_glTextureMap[size][unicodeChar] = glTexture;
		}

		glBindTexture(GL_TEXTURE_2D, glTexture);
		glCallLists(1, GL_UNSIGNED_BYTE, &glDisplayList);
	}
}
