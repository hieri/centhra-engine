#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- FreeType 2-
#include <ft2build.h>
#include FT_FREETYPE_H

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Font.h>

//TODO: handle freetype errors on startup
//TODO: get text dimensions

using namespace std;

FT_Library g_ftLibrary;

namespace ce
{
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

	Font::Font() : m_charWidth(0), m_charHeight(0), m_horizontalDpi(0), m_verticalDpi(0)
	{
		m_ftFace = 0;
	}
	Font::~Font()
	{
		DeleteDisplayData();
		FT_Done_Face((FT_Face)m_ftFace);
	}
	void Font::DeleteDisplayData()
	{
		for(map<unsigned short, unsigned int>::iterator it = m_glDisplayListMap.begin(); it != m_glDisplayListMap.end(); it++)
			glDeleteLists(it->second, 1);
		m_glDisplayListMap.clear();
		for(map<unsigned short, unsigned int>::iterator it = m_glUIDisplayListMap.begin(); it != m_glUIDisplayListMap.end(); it++)
				glDeleteLists(it->second, 1);
		m_glUIDisplayListMap.clear();
		for(map<unsigned short, unsigned int>::iterator it = m_glTextureMap.begin(); it != m_glTextureMap.end(); it++)
			glDeleteTextures(1, &it->second);
		m_glTextureMap.clear();
		m_displayInfoMap.clear();
	}
	void Font::SetDPI(float horizontalDpi, float verticalDpi)
	{
		bool updateCharSize = false;
		if(m_charWidth | m_charHeight)
			if(horizontalDpi != m_horizontalDpi || verticalDpi != m_verticalDpi)
				updateCharSize = true;

		m_horizontalDpi = horizontalDpi;
		m_verticalDpi = verticalDpi;

		if(updateCharSize)
		{
			FT_Face ftFace = (FT_Face)m_ftFace;
			//TODO: Handle floating point dpi
			FT_Set_Char_Size(ftFace, m_charWidth * 64, m_charHeight * 64, (FT_UInt)m_horizontalDpi, (FT_UInt)m_verticalDpi);
			DeleteDisplayData();
		}
	}
	void Font::SetCharacterDimensions(unsigned int height)
	{
		bool updateCharSize = false;
		if(m_horizontalDpi || m_verticalDpi)
			if(height != m_charHeight)
				updateCharSize = true;

		m_charHeight = height;

		if(updateCharSize)
		{
			FT_Face ftFace = (FT_Face)m_ftFace;
			//TODO: Handle floating point dpi
			FT_Set_Char_Size(ftFace, m_charWidth * 64, m_charHeight * 64, (FT_UInt)m_horizontalDpi, (FT_UInt)m_verticalDpi);
			DeleteDisplayData();
		}
	}
	void Font::GenerateDisplayAndInfo(unsigned short unicodeChar)
	{
		FT_Face ftFace = (FT_Face)m_ftFace;

		int error = FT_Load_Char(ftFace, unicodeChar, FT_LOAD_RENDER);
		FT_GlyphSlot &ftGlyph = ftFace->glyph;
		FT_Bitmap &ftBitmap = ftGlyph->bitmap;
		int width = ftBitmap.width;
		int height = ftBitmap.rows;

		int w = 1, h = 1;
		while(w < width)
			w <<= 1;
		while(h < height)
			h <<= 1;

		//- Generate Info -
		DisplayInfo info;
		info.bearingX = ftGlyph->bitmap_left;
		info.bearingY = ftGlyph->bitmap_top;
		info.width = ftBitmap.width;
		info.height = ftBitmap.rows;
		info.advance = ftGlyph->advance.x >> 6;
		m_displayInfoMap[unicodeChar] = info;

		//- Generate Texture -
		unsigned char *data = new unsigned char[2 * w * h];
		for(int a = 0; a < w; a++)
			for(int b = 0; b < h; b++)
			{
				int i = 2 * (a + b * w);
				data[i] = 255;
				data[i + 1] = (a >= width || b >= height) ? 0 : ftBitmap.buffer[a + b * width];
			}

		unsigned int glTexture = 0;
		glGenTextures(1, &glTexture);
		glBindTexture(GL_TEXTURE_2D, glTexture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
		delete [] data;

		m_glTextureMap[unicodeChar] = glTexture;

		//- Generate Regular Display List -
		unsigned int glDisplayList = glGenLists(1);
		glNewList(glDisplayList, GL_COMPILE);

			glPushMatrix();
				glTranslatef((GLfloat)ftGlyph->bitmap_left, 0, 0);
				glTranslatef(0, (GLfloat)ftGlyph->bitmap_top, 0);

				float x = (float)width / (float)w;
				float y = (float)height / (float)h;

				glBegin(GL_QUADS);
					glTexCoord2f(0, y); glVertex2f(0, (GLfloat)-ftBitmap.rows);
					glTexCoord2f(0, 0); glVertex2f(0, 0);
					glTexCoord2f(x, 0); glVertex2f((GLfloat)ftBitmap.width, 0);
					glTexCoord2f(x, y); glVertex2f((GLfloat)ftBitmap.width, (GLfloat)-ftBitmap.rows);
				glEnd();
			glPopMatrix();
			glTranslatef((GLfloat)info.advance, 0, 0);

		glEndList();

		m_glDisplayListMap[unicodeChar] = glDisplayList;

		//- Generate UI Display List -
		glDisplayList = glGenLists(1);
		glNewList(glDisplayList, GL_COMPILE);

			glPushMatrix();
				glTranslatef(0, (float)m_charHeight - (GLfloat)ftGlyph->bitmap_top, 0);
				glTranslatef((GLfloat)ftGlyph->bitmap_left, 0, 0);

				x = (float)width / (float)w;
				y = (float)height / (float)h;

				glBegin(GL_QUADS);
					glTexCoord2f(0, y); glVertex2f(0, (GLfloat)ftBitmap.rows);
					glTexCoord2f(0, 0); glVertex2f(0, 0);
					glTexCoord2f(x, 0); glVertex2f((GLfloat)ftBitmap.width, 0);
					glTexCoord2f(x, y); glVertex2f((GLfloat)ftBitmap.width, (GLfloat)ftBitmap.rows);
				glEnd();
			glPopMatrix();
			glTranslatef((GLfloat)info.advance, 0, 0);

		glEndList();

		m_glUIDisplayListMap[unicodeChar] = glDisplayList;
	}
	void Font::DrawString(const char *str, unsigned int newLineAdvance)
	{
		glPushMatrix();
			unsigned int len = strlen(str);
			for(unsigned int a = 0; a < len; a++)
			{
				if(str[a] == '\n')
				{
					glPopMatrix();
						glTranslatef(0, -(float)m_charHeight - (float)newLineAdvance, 0);
					glPushMatrix();
				}
				else
					DrawUnicodeChar(str[a]);
			}
		glPopMatrix();
	}
	void Font::DrawStringUI(const char *str, unsigned int newLineAdvance)
	{
		glPushMatrix();
			unsigned int len = strlen(str);
			for(unsigned int a = 0; a < len; a++)
			{
				if(str[a] == '\n')
				{
					glPopMatrix();
						glTranslatef(0, (float)m_charHeight + (float)newLineAdvance - 4, 0);
					glPushMatrix();
				}
				else
					DrawUnicodeCharUI(str[a]);
			}
		glPopMatrix();
	}
	void Font::DrawUnicodeChar(unsigned short unicodeChar)
	{
		glEnable(GL_TEXTURE_2D);

		if(!m_glDisplayListMap.count(unicodeChar))
			GenerateDisplayAndInfo(unicodeChar);

		unsigned int glTexture = m_glTextureMap[unicodeChar];
		glBindTexture(GL_TEXTURE_2D, glTexture);

		unsigned int glDisplayList = m_glDisplayListMap[unicodeChar];

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glCallLists(1, GL_UNSIGNED_BYTE, &glDisplayList);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}
	void Font::DrawUnicodeCharUI(unsigned short unicodeChar)
	{
		glEnable(GL_TEXTURE_2D);

		if(!m_glDisplayListMap.count(unicodeChar))
			GenerateDisplayAndInfo(unicodeChar);

		unsigned int glTexture = m_glTextureMap[unicodeChar];
		glBindTexture(GL_TEXTURE_2D, glTexture);

		unsigned int glDisplayList = m_glUIDisplayListMap[unicodeChar];

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glCallLists(1, GL_UNSIGNED_BYTE, &glDisplayList);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}
	unsigned int Font::GetCharHeight() const
	{
		return m_charHeight;
	}
	Vector2<int> Font::StringDimensions(const char *str, unsigned int newLineAdvance)
	{
		Vector2<int> dimensions, currentLine(0, m_charHeight);
		int lineIdx = 0;

		unsigned int len = strlen(str);
		for(unsigned int a = 0; a < len; a++)
		{
			unsigned short unicodeChar = str[a];

			if(unicodeChar == '\n')
			{
				if(lineIdx)
				{
				/*	if(currentLine[1] > m_charHeight)
					{
						int lead = m_charHeight + newLineAdvance;
						if(lead > currentLine[1])
							dimensions[1] += lead - currentLine[1];
					}
					else*/
					dimensions[1] += newLineAdvance;
				}

//				dimensions[1] += currentLine[1];
				dimensions[1] += m_charHeight;

				if(currentLine[0] > dimensions[0])
					dimensions[0] = currentLine[0];
				currentLine = Vector2<int>(0, 0);
				lineIdx++;
			}
			else
			{
				if(!m_displayInfoMap.count(unicodeChar))
					GenerateDisplayAndInfo(unicodeChar);
				
				DisplayInfo &info = m_displayInfoMap[unicodeChar];

//				print("Info: %d\t%d\t%d\t%d\t||\t%d\n", info.width, info.height, info.bearingX, info.bearingY, info.advance);

				currentLine[0] += info.advance;
				int curHeight = m_charHeight - info.bearingY + info.height;
				if(curHeight > currentLine[1])
					currentLine[1] = curHeight;
			}
		}

		if((unsigned int)currentLine[1] > m_charHeight)
			dimensions[1] += currentLine[1];
		else
			dimensions[1] += m_charHeight;

		if(currentLine[0] > dimensions[0])
			dimensions[0] = currentLine[0];

		return dimensions;
	}
}
