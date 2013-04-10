//- FreeType 2-
#include <ft2build.h>
#include FT_FREETYPE_H

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Font.h>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- TODO: handle freetype errors on startup -

using namespace std;

FT_Library g_ftLibrary;

namespace ce
{
	Font *Font::ms_default;
	bool Font::Init()
	{
		if(!FT_Init_FreeType(&g_ftLibrary))
			return false;
		return false;
	}
	Font *Font::CreateFromFile(const char *file)
	{
		FT_Face ftFace;
		int error = FT_New_Face(g_ftLibrary, file, 0, &ftFace);
		if(error)
		{
			switch(error)
			{
			case FT_Err_Unknown_File_Format:
				setError("FT_New_Face: Unknown file format <%s>.", file);
				break;
			}
			delete ftFace;
			return 0;
		}

		Font *font = new Font();
		font->m_ftFace = ftFace;
		return font;
	}
	Font *Font::CreateFromMemory(const unsigned char *memory, unsigned int size)
	{
		FT_Face ftFace;
		int error = FT_New_Memory_Face(g_ftLibrary, memory, size, 0, &ftFace);
		if(error)
		{
			delete ftFace;
			return 0;
		}

		Font *font = new Font();
		font->m_ftFace = font;
		return font;
	}

	Font::Font()
	{
		m_ftFace = 0;
		m_height = 0;
	}
	Font::~Font()
	{
		for(map<unsigned int, map<unsigned short, unsigned int> >::iterator itA = m_glDisplayListMap.begin(); itA != m_glDisplayListMap.end(); itA++)
			for(map<unsigned short, unsigned int>::iterator itB = itA->second.begin(); itB != itA->second.end(); itB++)
				glDeleteLists(itB->second, 1);
		for(map<unsigned int, map<unsigned short, unsigned int> >::iterator itA = m_glTextureMap.begin(); itA != m_glTextureMap.end(); itA++)
			for(map<unsigned short, unsigned int>::iterator itB = itA->second.begin(); itB != itA->second.end(); itB++)
				glDeleteTextures(1, &itB->second);
		delete (FT_Face)m_ftFace;
	}
	void Font::SetCharSize(unsigned int width, unsigned int height, unsigned int horizontalDpi, unsigned int verticalDpi)
	{
		m_height = height;
		FT_Face ftFace = (FT_Face)m_ftFace;
		FT_Set_Char_Size(ftFace, width, height, horizontalDpi, verticalDpi);
	}
	void Font::DrawString(const char *str)
	{
		for(unsigned int a = 0; a < strlen(str); a++)
			DrawUnicodeChar(str[a]);
	}
	void Font::DrawUnicodeChar(unsigned short unicodeChar)
	{
		FT_Face ftFace = (FT_Face)m_ftFace;

		int error = FT_Load_Char(ftFace, unicodeChar, FT_LOAD_RENDER);
		FT_GlyphSlot &ftGlyph = ftFace->glyph;
		FT_Bitmap &ftBitmap = ftGlyph->bitmap;
		int width = ftBitmap.width;
		int height = ftBitmap.rows;

		glEnable(GL_TEXTURE_2D);

		unsigned int glTexture = 0;
		if(m_glTextureMap[m_height].count(unicodeChar))
		{
			glTexture = m_glTextureMap[m_height][unicodeChar];
			glBindTexture(GL_TEXTURE_2D, glTexture);
		}
		else
		{
			int w = 1, h = 1;
			while(w < width)
				w <<= 1;
			while(h < height)
				h <<= 1;

			unsigned char *data = new unsigned char[2 * w * h];
			for(int a = 0; a < w; a++)
				for(int b = 0; b < h; b++)
				{
					int i = 2 * (a + b * w);
					data[i] = 255;
					data[i + 1] = (a >= width || b >= height) ? 0 : ftBitmap.buffer[a + b * width];
				}
			glGenTextures(1, &glTexture);
			glBindTexture(GL_TEXTURE_2D, glTexture);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
			delete [] data;

			m_glTextureMap[m_height][unicodeChar] = glTexture;
		}

		unsigned int glDisplayList = 0;
		if(m_glDisplayListMap[m_height].count(unicodeChar))
			glDisplayList = m_glDisplayListMap[m_height][unicodeChar];
		else
		{
			int w = 1, h = 1;
			while(w < width)
				w <<= 1;
			while(h < height)
				h <<= 1;

			glDisplayList = glGenLists(1);
			glNewList(glDisplayList, GL_COMPILE);
				glPushMatrix();
					glTranslatef(ftGlyph->bitmap_left, 0, 0);
					glTranslatef(0, ftGlyph->bitmap_top, 0);

					float x = (float)width / (float)w;
					float y = (float)height / (float)h;

					glBegin(GL_QUADS);
						glTexCoord2f(0, y); glVertex2f(0, -ftBitmap.rows);
						glTexCoord2f(0, 0); glVertex2f(0, 0);
						glTexCoord2f(x, 0); glVertex2f(ftBitmap.width, 0);
						glTexCoord2f(x, y); glVertex2f(ftBitmap.width, -ftBitmap.rows);

/*						glTexCoord2f(0, 0); glVertex2f(0, ftBitmap.rows);
						glTexCoord2f(0, y); glVertex2f(0, 0);
						glTexCoord2f(x, y); glVertex2f(ftBitmap.width, 0);
						glTexCoord2f(x, 0); glVertex2f(ftBitmap.width, ftBitmap.rows);
*/
					glEnd();
				glPopMatrix();
				glTranslatef(ftGlyph->advance.x >> 6, 0, 0);
			glEndList();

			m_glDisplayListMap[m_height][unicodeChar] = glDisplayList;
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCallLists(1, GL_UNSIGNED_BYTE, &glDisplayList);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}
}
