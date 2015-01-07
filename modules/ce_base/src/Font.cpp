//- Freeunsigned char 2-
#include <ft2build.h>
#include FT_FREETYPE_H

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Font.h>
#include <CE/Image.h>
#include <CE/Renderer.h>

//- Standard Library -
#include <cstring>
#include <vector>

//TODO: handle freetype errors on startup
//TODO: get text dimensions
//TODO: Unicode char rendering

using namespace std;

namespace ce
{
	//-------------------------------- Globals --------------------------------
	FT_Library g_ftLibrary;

	//-------------------------------- Font --------------------------------
	bool Font::Init()
	{
		if(!FT_Init_FreeType(&g_ftLibrary))
			return false;
		return false;
	}
	Font *Font::CreateFromFile(const char *file, unsigned int charSize, float horizontalDpi, float verticalDpi)
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
		return new Font(ftFace, charSize, horizontalDpi, verticalDpi);
	}
	Font *Font::CreateFromMemory(const unsigned char *memory, unsigned int size, unsigned int charSize, float horizontalDpi, float verticalDpi)
	{
		FT_Face ftFace;
		int error = FT_New_Memory_Face(g_ftLibrary, memory, size, 0, &ftFace);
		if(error)
		{
			delete ftFace;
			return 0;
		}
		return new Font(ftFace, charSize, horizontalDpi, verticalDpi);
	}

	Font::Font(void *face, unsigned int charSize, float horizontalDpi, float verticalDpi)
		: m_ftFace(face), m_charSize(charSize), m_horizontalDpi(horizontalDpi), m_verticalDpi(verticalDpi),
		m_glBuffer(0), m_glTexture(0)
	{
		//- Initialize Font Face -
		FT_Face ftFace = (FT_Face)m_ftFace;
		FT_Set_Char_Size(ftFace, 0, m_charSize * 64, (FT_UInt)m_horizontalDpi, (FT_UInt)m_verticalDpi);

		GenerateDisplayData();
	}
	Font::~Font()
	{
		DeleteDisplayData();
		FT_Done_Face((FT_Face)m_ftFace);
	}
	void Font::GenerateDisplayData()
	{
		FT_Face ftFace = (FT_Face)m_ftFace;
		FT_GlyphSlot &ftGlyph = ftFace->glyph;
		FT_Bitmap &ftBitmap = ftGlyph->bitmap;

		//- Fill in ASCII -
		{
			int fixedHeight = 0;
			int fixedWidth = 0;
			int numChars = 0;

			vector<unsigned char *> imageData;
			imageData.reserve(127 - 32);
			int error;

			//- Load Image Data -
			for(unsigned short c = 32; c < 127; c++)
			{
				error = FT_Load_Char(ftFace, c, FT_LOAD_RENDER);
				if(error != 0)
				{
					//TODO: Handle error
					continue;
				}

				//- Generate Info -
				DisplayInfo info;
				info.bearingX = ftGlyph->bitmap_left;
				info.bearingY = ftGlyph->bitmap_top;
				info.width = ftBitmap.width;
				info.height = ftBitmap.rows;
				info.advance = (int_canvas)(ftGlyph->advance.x >> 6);
				info.advanceMatrix = Matrix4x4<float>::BuildFromTranslation(Vector2<int_canvas>(info.advance, 0));

				if(fixedWidth < info.width)
					fixedWidth = info.width;
				if(fixedHeight < info.height)
					fixedHeight = info.height;
				numChars++;

				//- Load image data into temporary buffer. -
				unsigned char *data = new unsigned  char[info.width * info.height];
				for(int a = 0; a < info.width; a++)
					for(int b = 0; b < info.height; b++)
					{
						int i = (a + b * info.width);
						data[i] = ftBitmap.buffer[i];
					}
				imageData.push_back(data);

				m_displayInfoMap[c] = info;
			}

			//- Generate the master image. -
			unsigned int d = 1;
			{
				unsigned int area = fixedWidth * fixedHeight * numChars;
				unsigned int _d = (unsigned int)ceil(sqrtf((float)area));
				while(d < _d)
					d <<= 1;
			}

			int size = d * d;
			int imageDataSize = size * 2;
			unsigned char *data = new unsigned char[imageDataSize];
			float *buffer = new float[size * 16];
			float fD = (float)d;
			float fCharSize = (float)m_charSize;
			memset(data, 0, imageDataSize);
			int _x = 0;
			int _y = 0;
			for(unsigned short c = 32, idx = 0, _c = 0; c < 127; ++c, ++_c)
			{
				DisplayInfo &info = m_displayInfoMap[c];
				info.offset = idx * 4;

				//- Copy Image Data -
				for(int y = 0; y < info.height; y++)
					for(int x = 0; x < info.width; x++)
					{
						int i = 2 * ((_x + x) + (_y + y) * d);
						data[i] = 255;
						data[i + 1] = imageData[_c][x + y * info.width];
					}

				//- Populate the Buffer -
				float vW = (float)info.width;
				float vH = (float)info.height;
				float vX = (float)info.bearingX;
				float vY = fCharSize - (float)info.bearingY;

				float tX = (float)_x / fD;
				float tY = (float)_y / fD;
				float tW = vW / fD;
				float tH = vH / fD;

				buffer[idx] = vX;
				buffer[idx + 1] = vY + vH;
				buffer[idx + 2] = tX;
				buffer[idx + 3] = tY + tH;

				buffer[idx + 4] = vX;
				buffer[idx + 5] = vY;
				buffer[idx + 6] = tX;
				buffer[idx + 7] = tY;

				buffer[idx + 8] = vX + vW;
				buffer[idx + 9] = vY;
				buffer[idx + 10] = tX + tW;
				buffer[idx + 11] = tY;

				buffer[idx + 12] = vX + vW;
				buffer[idx + 13] = vY + vH;
				buffer[idx + 14] = tX + tW;
				buffer[idx + 15] = tY + tH;

				idx += 16;

				_x += fixedWidth;
				if((_x + info.width) >= (int)d)
				{
					_x = 0;
					_y += fixedHeight;
				}
			}

			m_glBuffer = 0;
			glGenBuffers(1, &m_glBuffer);
			BindVBO(m_glBuffer);
			glBufferData(0x8892, size * 16 * 4, buffer, 0x88E4);

			m_glTexture = 0;
			glGenTextures(1, &m_glTexture);
			glBindTexture(GL_TEXTURE_2D, m_glTexture);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, d, d, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);

			//- Clean up temporary image data. -
			if(imageData.empty() == false)
			{
				unsigned char **markData = &imageData.front();
				unsigned char **endData = markData + imageData.size();
				while(markData != endData)
					delete[] * markData++;
			}
			delete[] buffer;
			delete[] data;
		}
	}
	void Font::DeleteDisplayData()
	{
		if(m_glBuffer)
			glDeleteBuffers(1, &m_glBuffer);
		if(m_glTexture)
			glDeleteTextures(1, &m_glTexture);
		m_displayInfoMap.clear();
	}
	void Font::SetDPI(float horizontalDpi, float verticalDpi)
	{
		if(horizontalDpi != m_horizontalDpi || verticalDpi != m_verticalDpi)
		{
			m_horizontalDpi = horizontalDpi;
			m_verticalDpi = verticalDpi;

			FT_Face ftFace = (FT_Face)m_ftFace;
			//TODO: Handle floating point dpi
			FT_Set_Char_Size(ftFace, 0, m_charSize * 64, (FT_UInt)m_horizontalDpi, (FT_UInt)m_verticalDpi);

			DeleteDisplayData();
			GenerateDisplayData();
		}
	}
	void Font::DrawString(RenderContext &context, const char *str, unsigned int newLineAdvance)
	{
		//TODO: World string drawing
	}
	void Font::DrawStringUI(RenderContext &context, const char *str, unsigned int newLineAdvance)
	{
		unsigned int len = strlen(str);

		glBindTexture(GL_TEXTURE_2D, m_glTexture);
		BindVBO(m_glBuffer);
		BindSquareEBO();

		ConfigTextured();

		ShaderProgram::TexturedProgram *program = 0;
		if(context.useShaders)
			program = UseTexturedProgram();
		if(program != 0)
		{
			Matrix4x4<float> mvpMatrix = context.mvpMatrix;
			Matrix4x4<float> currentMVP = mvpMatrix;
			for(unsigned int a = 0; a < len; a++)
			{
				char c = str[a];
				if(c == '\n')
				{
					mvpMatrix *= Matrix4x4<float>::BuildFromTranslation(Vector2<int_canvas>(0, m_charSize + newLineAdvance));
					currentMVP = mvpMatrix;
				}
				else
				{
					if(m_displayInfoMap.count(c) == 0)
						continue;
					DisplayInfo &info = m_displayInfoMap[c];
					glUniformMatrix4fv(program->mvpMatrix, 1, GL_FALSE, &currentMVP[0]);
					glVertexAttribPointer(program->position, 2, GL_FLOAT, 0, 16, (GLvoid *)info.offset);
					glVertexAttribPointer(program->uv, 2, GL_FLOAT, 0, 16, (GLvoid *)(info.offset + 8));
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
					currentMVP *= info.advanceMatrix;
				}
			}
		}
		else
		{
			glPushMatrix();
			for(unsigned int a = 0; a < len; a++)
			{
				char c = str[a];
				if(c == '\n')
				{
					glPopMatrix();
					glTranslatef(0, (float)m_charSize + (float)newLineAdvance, 0);
					glPushMatrix();
				}
				else
				{
					if(m_displayInfoMap.count(c) == 0)
						continue;
					DisplayInfo &info = m_displayInfoMap[c];
					glVertexPointer(2, GL_FLOAT, 16, (GLvoid *)info.offset);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glTexCoordPointer(2, GL_FLOAT, 16, (GLvoid *)(info.offset + 8));
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					glMultMatrixf(&info.advanceMatrix[0]);
				}
			}
			glPopMatrix();
		}
	}
	void Font::DrawUnicodeChar(RenderContext &context, unsigned short unicodeChar)
	{
		//TODO: World character drawing
	}
	void Font::DrawUnicodeCharUI(RenderContext &context, unsigned short unicodeChar)
	{
		if(m_displayInfoMap.count(unicodeChar) == 0)
			return;

		ConfigTextured();

		glBindTexture(GL_TEXTURE_2D, m_glTexture);
		BindVBO(m_glBuffer);
		BindSquareEBO();

		ShaderProgram::TexturedProgram *program = 0;
		if(context.useShaders)
			program = UseTexturedProgram();
		if(program != 0)
		{
			Matrix4x4<float> mvpMatrix = context.mvpMatrix;
			Matrix4x4<float> currentMVP = mvpMatrix;
			DisplayInfo &info = m_displayInfoMap[unicodeChar];
			glUniformMatrix4fv(program->mvpMatrix, 1, GL_FALSE, &currentMVP[0]);
			glVertexAttribPointer(program->position, 2, GL_FLOAT, 0, 16, (GLvoid *)info.offset);
			glVertexAttribPointer(program->uv, 2, GL_FLOAT, 0, 16, (GLvoid *)(info.offset + 8));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
			currentMVP *= info.advanceMatrix;
		}
		else
		{
			DisplayInfo &info = m_displayInfoMap[unicodeChar];
			glVertexPointer(2, GL_FLOAT, 16, (GLvoid *)info.offset);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 16, (GLvoid *)(info.offset + 8));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}
	Vector2<int> Font::StringDimensions(const char *str, unsigned int newLineAdvance)
	{
		Vector2<int> dimensions, currentLine(0, m_charSize);
		int lineIdx = 0;

		unsigned int len = strlen(str);
		for(unsigned int a = 0; a < len; a++)
		{
			unsigned short unicodeChar = str[a];

			if(unicodeChar == '\n')
			{
				if(lineIdx)
				{
				/*	if(currentLine[1] > m_charSize)
					{
						int lead = m_charSize + newLineAdvance;
						if(lead > currentLine[1])
							dimensions[1] += lead - currentLine[1];
					}
					else*/
					dimensions[1] += newLineAdvance;
				}

//				dimensions[1] += currentLine[1];
				dimensions[1] += m_charSize;

				if(currentLine[0] > dimensions[0])
					dimensions[0] = currentLine[0];
				currentLine = Vector2<int>(0, 0);
				lineIdx++;
			}
			else
			{
				DisplayInfo &info = m_displayInfoMap[unicodeChar];

//				print("Info: %d\t%d\t%d\t%d\t||\t%d\n", info.width, info.height, info.bearingX, info.bearingY, info.advance);

				currentLine[0] += info.advance;
				int curHeight = m_charSize - info.bearingY + info.height;
				if(curHeight > currentLine[1])
					currentLine[1] = curHeight;
			}
		}

		if((unsigned int)currentLine[1] > m_charSize)
			dimensions[1] += currentLine[1];
		else
			dimensions[1] += m_charSize;

		if(currentLine[0] > dimensions[0])
			dimensions[0] = currentLine[0];

		return dimensions;
	}

	//------------ Character Size ------------

	/**	@brief Sets the character size of the Font.
	*	@param size The size of a character.
	*/
	void Font::SetCharacterSize(unsigned int size)
	{
		if(m_horizontalDpi || m_verticalDpi)
			if(size != m_charSize)
			{
				m_charSize = size;

				FT_Face ftFace = (FT_Face)m_ftFace;
				//TODO: Handle floating point dpi
				FT_Set_Char_Size(ftFace, 0, m_charSize * 64, (FT_UInt)m_horizontalDpi, (FT_UInt)m_verticalDpi);

				DeleteDisplayData();
				GenerateDisplayData();
			}
	}

	/**	@brief Returns the character size of the font.
	*	@return The character size of the Font.
	*/
	unsigned int Font::GetCharSize() const
	{
		return m_charSize;
	}
}
