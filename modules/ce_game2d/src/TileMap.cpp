#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Game2D/TileMap.h>
#include <CE/Base.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		TileMap::TileMap(Vector2<unsigned short> size, Vector2<unsigned short> tileSize, TileSet *tileSet) :
			m_size(size), m_tileSize(tileSize), m_tileSet(tileSet),
			m_bleedLevel(0.f)
		{
			m_tiles = new Vector2<unsigned char> *[size[0]];
			for(unsigned short a = 0; a < size[0]; a++)
			{
				m_tiles[a] = new Vector2<unsigned char>[size[1]];
				for(unsigned short b = 0; b < size[1]; b++)
					m_tiles[a][b] = Vector2<unsigned char>(255, 255);
			}
		}
		TileMap::~TileMap()
		{
			for(unsigned short a = 0; a < m_size[0]; a++)
				delete [] m_tiles[a];
			delete [] m_tiles;
		}
		Vector2<unsigned short> TileMap::GetSize() const
		{
			return m_size;
		}
		Vector2<unsigned short> TileMap::GetTileSize() const
		{
			return m_tileSize;
		}
		void TileMap::Render(unsigned short minX, unsigned short minY, unsigned short maxX, unsigned short maxY)
		{
			glPushMatrix();

			if(minX >= m_size[0])
				minX = m_size[0] - 1;
			if(minY >= m_size[1])
				minY = m_size[1] - 1;
			if(maxX >= m_size[0])
				maxX = m_size[0] - 1;
			if(maxY >= m_size[1])
				maxY = m_size[1] - 1;

			glEnable(GL_TEXTURE_2D);

			Vector2<unsigned short> tileSetSize = m_tileSet->GetSize();
			Image *tileSetImage = m_tileSet->GetImage();
			Vector2<unsigned int> tileSetImageSize = tileSetImage->GetSize();
			tileSetImage->Bind();

			glBegin(GL_QUADS);
			for(unsigned int a = minX; a <= maxX; a++)
				for(unsigned int b = minY; b <= maxY; b++)
				{
					Vector2<unsigned char> &tile = m_tiles[a][b];
					if(tile[0] < tileSetSize[0] && tile[1] < tileSetSize[1])
					{
						unsigned int x, y, X, Y, _x, _y, _X, _Y;
						x = a * m_tileSize[0];
						y = b * m_tileSize[1];
						X = x + m_tileSize[0];
						Y = y + m_tileSize[1];

						_x = tile[0] * m_tileSize[0];
						_y = tile[1] * m_tileSize[1];
						_X = _x + m_tileSize[0];
						_Y = _y + m_tileSize[1];


						float tx, ty, tX, tY;
						tx = ((float)_x + m_bleedLevel) / ((float)tileSetImageSize[0]);
						ty = ((float)_y + m_bleedLevel) / ((float)tileSetImageSize[1]);
						tX = ((float)_X - m_bleedLevel) / ((float)tileSetImageSize[0]);
						tY = ((float)_Y - m_bleedLevel) / ((float)tileSetImageSize[1]);

						glTexCoord2f(tx, tY);
						glVertex2i(x, y);
						glTexCoord2f(tX, tY);
						glVertex2i(X, y);
						glTexCoord2f(tX, ty);
						glVertex2i(X, Y);
						glTexCoord2f(tx, ty);
						glVertex2i(x, Y);
					}
				}
			glEnd();

			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}

		//- Tiles -
		Vector2<unsigned char> TileMap::GetTile(unsigned short x, unsigned short y) const
		{
			return m_tiles[x][y];
		}
		void TileMap::SetTile(unsigned short x, unsigned short y, Vector2<unsigned char> value)
		{
			m_tiles[x][y] = value;
		}

		//- TileSet -
		TileSet *TileMap::GetTileSet() const
		{
			return m_tileSet;
		}
		void TileMap::SetTileSet(TileSet *tileSet)
		{
			m_tileSet = tileSet;
		}

		//- Bleeding -
		float TileMap::GetBleedLevel() const
		{
			return m_bleedLevel;
		}
		void TileMap::SetBleedLevel(float bleedLevel)
		{
			m_bleedLevel = bleedLevel;
		}
	}
}
