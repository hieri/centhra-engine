#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Game2D/SimpleTileMap.h>
#include <CE/Base.h>

using namespace std;

//TODO: Account for camera bounds during rendering

namespace ce
{
	namespace game2d
	{
		SimpleTileMap::SimpleTileMap(Vector2<unsigned short> size, Vector2<unsigned short> tileSize, Image *tileSet) : TileMap(size, tileSize)
		{
			m_tiles = new Vector2<unsigned char> *[size[0]];
			for(unsigned short a = 0; a < size[0]; a++)
				m_tiles[a] = new Vector2<unsigned char>[size[1]];

			m_tileSet = tileSet;
		}
		SimpleTileMap::~SimpleTileMap()
		{
			for(unsigned short a = 0; a < m_size[0]; a++)
				delete [] m_tiles[a];
			delete [] m_tiles;
		}
		Vector2<unsigned char> SimpleTileMap::GetTile(unsigned short x, unsigned short y) const
		{
			return m_tiles[x][y];
		}
		void SimpleTileMap::SetTile(unsigned short x, unsigned short y, Vector2<unsigned char> value)
		{
			m_tiles[x][y] = value;
		}
		void SimpleTileMap::Render(unsigned short minX, unsigned short minY, unsigned short maxX, unsigned short maxY)
		{
			glPushMatrix();

			Vector2<unsigned int> size = m_tileSet->GetSize();
			Vector2<unsigned int> max = size;
			Vector2<unsigned short> mapSize = GetSize();
			max[0] /= m_tileSize[0];
			max[1] /= m_tileSize[1];
			
			if(minX >= mapSize[0])
				minX = mapSize[0] - 1;
			if(minY >= mapSize[1])
				minY = mapSize[1] - 1;
			if(maxX >= mapSize[0])
				maxX = mapSize[0] - 1;
			if(maxY >= mapSize[1])
				maxY = mapSize[1] - 1;

			glEnable(GL_TEXTURE_2D);
			m_tileSet->Bind();

			glBegin(GL_QUADS);
			for(unsigned short a = minX; a < maxX; a++)
				for(unsigned short b = minY; b < maxY; b++)
					if(m_tiles[a][b][0] < max[0] && m_tiles[a][b][1] < max[1])
					{
						unsigned short x, y, X, Y, _x, _y, _X, _Y;
						x = a * m_tileSize[0];
						y = b * m_tileSize[1];
						X = x + m_tileSize[0];
						Y = y + m_tileSize[1];

						_x = m_tiles[a][b][0] * m_tileSize[0];
						_y = m_tiles[a][b][1] * m_tileSize[1];
						_X = _x + m_tileSize[0];
						_Y = _y + m_tileSize[1];

						float tx, ty, tX, tY;
						tx = ((float)_x + 0.5f) / ((float)size[0]);
						ty = ((float)_y + 0.5f) / ((float)size[1]);
						tX = ((float)_X - 0.5f) / ((float)size[0]);
						tY = ((float)_Y - 0.5f) / ((float)size[1]);

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
	}
}
