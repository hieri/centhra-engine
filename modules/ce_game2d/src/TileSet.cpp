//- Centhra Engine -
#include <CE/Game2D/TileSet.h>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		TileSet::TileSet(Image *sourceImage, Vector2<unsigned short> tileSize)
		{
			m_sourceImage = sourceImage;
			m_tileSize = tileSize;

			Vector2<unsigned int> imageSize = sourceImage->GetSize();
			m_size = Vector2<unsigned char>((unsigned char)(imageSize[0] / tileSize[0]), (unsigned char)(imageSize[1] / tileSize[1]));
		}
		string TileSet::GetName() const
		{
			return m_name;
		}
		void TileSet::SetName(string name)
		{
			m_name = name;
		}
		Image *TileSet::GetImage() const
		{
			return m_sourceImage;
		}
		Vector2<unsigned short> TileSet::GetTileSize() const
		{
			return m_tileSize;
		}
		Vector2<unsigned char> TileSet::GetSize() const
		{
			return m_size;
		}
		void TileSet::Render(unsigned char x, unsigned char y)
		{
			if(x < m_size[0] && y < m_size[1])
			{
				unsigned int _x, _y, _X, _Y;
				_x = x * m_tileSize[0];
				_y = y * m_tileSize[1];
				_X = _x + m_tileSize[0];
				_Y = _y + m_tileSize[1];

				Vector2<unsigned int> tileSetImageSize = m_sourceImage->GetSize();

				float tx, ty, tX, tY;
				tx = ((float)_x) / ((float)tileSetImageSize[0]);
				ty = ((float)_y) / ((float)tileSetImageSize[1]);
				tX = ((float)_X) / ((float)tileSetImageSize[0]);
				tY = ((float)_Y) / ((float)tileSetImageSize[1]);

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_TEXTURE_2D);

					m_sourceImage->Bind();
					glBegin(GL_QUADS);
						glTexCoord2f(tx, ty);
						glVertex2i(0, 1);

						glTexCoord2f(tX, ty);
						glVertex2i(1, 1);

						glTexCoord2f(tX, tY);
						glVertex2i(1, 0);

						glTexCoord2f(tx, tY);
						glVertex2i(0, 0);
					glEnd();

				glDisable(GL_TEXTURE_2D);
				glDisable(GL_BLEND);
			}
		}
		void TileSet::UIRender(unsigned char x, unsigned char y)
		{
			if(x < m_size[0] && y < m_size[1])
			{
				unsigned int _x, _y, _X, _Y;
				_x = x * m_tileSize[0];
				_y = y * m_tileSize[1];
				_X = _x + m_tileSize[0];
				_Y = _y + m_tileSize[1];

				Vector2<unsigned int> tileSetImageSize = m_sourceImage->GetSize();

				float tx, ty, tX, tY;
				tx = ((float)_x) / ((float)tileSetImageSize[0]);
				ty = ((float)_y) / ((float)tileSetImageSize[1]);
				tX = ((float)_X) / ((float)tileSetImageSize[0]);
				tY = ((float)_Y) / ((float)tileSetImageSize[1]);

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_TEXTURE_2D);

					m_sourceImage->Bind();
					glBegin(GL_QUADS);
						glTexCoord2f(tx, tY);
						glVertex2i(0, 1);

						glTexCoord2f(tX, tY);
						glVertex2i(1, 1);

						glTexCoord2f(tX, ty);
						glVertex2i(1, 0);

						glTexCoord2f(tx, ty);
						glVertex2i(0, 0);
					glEnd();

				glDisable(GL_TEXTURE_2D);
				glDisable(GL_BLEND);
			}
		}
	}
}
