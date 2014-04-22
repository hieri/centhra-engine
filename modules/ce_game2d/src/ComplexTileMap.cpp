//- Centhra Engine -
#include <CE/Game2D/ComplexTileMap.h>
#include <CE/Base.h>

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
		ComplexTileMap::Tile::Tile(TileSet *tileSet, Vector2<unsigned char> value)
		{
			m_tileSet = tileSet;
			m_value = value;
		}

		ComplexTileMap::ComplexTileMap(Vector2<unsigned short> size, Vector2<unsigned short> tileSize)
		{
			m_size = size;
			m_tileSize = tileSize;

			m_tiles = new vector<Tile *> *[size[0]];
			for(unsigned short a = 0; a < size[0]; a++)
				m_tiles[a] = new vector<Tile *>[size[1]];
		}
		ComplexTileMap::~ComplexTileMap()
		{
			for(unsigned short a = 0; a < m_size[0]; a++)
			{
				for(unsigned short b = 0; b < m_size[1]; b++)
					for(vector<ComplexTileMap::Tile *>::iterator it = m_tiles[a][b].begin(); it != m_tiles[a][b].end(); it++)
						delete *it;
				delete [] m_tiles[a];
			}
			delete [] m_tiles;
		}
		Vector2<unsigned short> ComplexTileMap::GetSize() const
		{
			return m_size;
		}
		Vector2<unsigned short> ComplexTileMap::GetTileSize() const
		{
			return m_tileSize;
		}
		std::vector<ComplexTileMap::Tile *> *ComplexTileMap::GetTiles(unsigned short x, unsigned short y) const
		{
			return &m_tiles[x][y];
		}
		void ComplexTileMap::AddTile(unsigned short x, unsigned short y, TileSet *tileSet, Vector2<unsigned char> value)
		{
			Tile *tile = new Tile(tileSet, value);
			m_tiles[x][y].push_back(tile);
		}
		void ComplexTileMap::Render(unsigned short minX, unsigned short minY, unsigned short maxX, unsigned short maxY)
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

			Image *lastImage = 0;
			glBegin(GL_QUADS);
			for(unsigned int a = minX; a < maxX; a++)
				for(unsigned int b = minY; b < maxY; b++)
				{
					for(vector<ComplexTileMap::Tile *>::iterator it = m_tiles[a][b].begin(); it != m_tiles[a][b].end(); it++)
					{
						Tile *tile = *it;
						TileSet *tileSet = tile->m_tileSet;
						if(tileSet)
						{
							Vector2<unsigned short> tileSetSize = tileSet->GetSize();
							if(tile->m_value[0] < tileSetSize[0] && tile->m_value[1] < tileSetSize[1])
							{
								unsigned int x, y, X, Y, _x, _y, _X, _Y;
								x = a * m_tileSize[0];
								y = b * m_tileSize[1];
								X = x + m_tileSize[0];
								Y = y + m_tileSize[1];

								_x = tile->m_value[0] * m_tileSize[0];
								_y = tile->m_value[1] * m_tileSize[1];
								_X = _x + m_tileSize[0];
								_Y = _y + m_tileSize[1];

								Image *tileSetImage = tileSet->GetImage();
								Vector2<unsigned int> tileSetImageSize = tileSetImage->GetSize();

								float tx, ty, tX, tY;
								tx = ((float)_x) / ((float)tileSetImageSize[0]);
								ty = ((float)_y) / ((float)tileSetImageSize[1]);
								tX = ((float)_X) / ((float)tileSetImageSize[0]);
								tY = ((float)_Y) / ((float)tileSetImageSize[1]);

								if(lastImage != tileSetImage)
								{
									glEnd();
									tileSetImage->Bind();
									glBegin(GL_QUADS);
									lastImage = tileSetImage;
								}

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
					}
				}
			glEnd();

			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
	}
}
