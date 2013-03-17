//- Centhra Engine -
#include <CE/Game2D/TileMap.h>
#include <CE/Base.h>

//- OpenGL -
#include <GL/gl.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		TileMap::TileMap(Vector2<unsigned int> size, Vector2<unsigned int> tileSize, Image *tileSet)
		{
			m_tiles = new Vector2<unsigned char> *[size[0]];
			for(unsigned int a = 0; a < size[0]; a++)
				m_tiles[a] = new Vector2<unsigned char>[size[1]];

			m_size = size;
			m_tileSize = tileSize;
			m_tileSet = tileSet;
		}
		TileMap::~TileMap()
		{
			for(unsigned int a = 0; a < m_size[0]; a++)
				delete [] m_tiles[a];
			delete [] m_tiles;
		}
		void TileMap::DoRender()
		{
			glPushMatrix();

			Vector2<unsigned int> size = m_tileSet->GetSize();
			Vector2<unsigned int> max = size;
			max[0] /= m_tileSize[0];
			max[1] /= m_tileSize[1];

			glEnable(GL_TEXTURE_2D);
			m_tileSet->Bind();

			glBegin(GL_QUADS);
			for(unsigned int a = 0; a < m_size[0]; a++)
				for(unsigned int b = 0; b < m_size[1]; b++)
					if(m_tiles[a][b][0] < max[0] && m_tiles[a][b][1] < max[1])
					{
						unsigned int x, y, X, Y, _x, _y, _X, _Y;
						x = a * m_tileSize[0];
						y = b * m_tileSize[1];
						X = x + m_tileSize[0];
						Y = y + m_tileSize[1];

						_x = m_tiles[a][b][0] * m_tileSize[0];
						_y = m_tiles[a][b][1] * m_tileSize[1];
						_X = _x + m_tileSize[0];
						_Y = _y + m_tileSize[1];

						float tx, ty, tX, tY;
						tx = ((float)_x) / ((float)size[0]);
						ty = ((float)_y) / ((float)size[1]);
						tX = ((float)_X) / ((float)size[0]);
						tY = ((float)_Y) / ((float)size[1]);

						glTexCoord2f(tx, tY);
						glVertex2i(x, y);
						glTexCoord2f(tX, tY);
						glVertex2i(X, y);
						glTexCoord2f(tX, ty);
						glVertex2i(X, Y);
						glTexCoord2f(tx, ty);
						glVertex2i(x, Y);
					}
			glEnd();

			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
		Vector2<unsigned int> TileMap::GetSize() const
		{
			return m_size;
		}
		Vector2<unsigned int> TileMap::GetTileSize() const
		{
			return m_tileSize;
		}
		Vector2<unsigned char> TileMap::GetTile(unsigned int x, unsigned int y) const
		{
			return m_tiles[x][y];
		}
		void TileMap::SetTile(unsigned int x, unsigned int y, Vector2<unsigned char> value)
		{
			m_tiles[x][y] = value;
		}
	}
}
