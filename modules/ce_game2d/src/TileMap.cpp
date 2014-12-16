//- Standard Library -
#include <algorithm>

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
		TileMap::TileMap(Vector2<unsigned short> size, Vector2<unsigned short> tileSize) :
			m_size(size), m_tileSize(tileSize),
			m_bleedLevel(0.f)
		{
			m_tiles = new Vector3<unsigned char> *[size[0]];
			for(unsigned short a = 0; a < size[0]; a++)
			{
				m_tiles[a] = new Vector3<unsigned char>[size[1]];
				for(unsigned short b = 0; b < size[1]; b++)
					m_tiles[a][b] = Vector3<unsigned char>(255, 255, 255);
			}
		}
		TileMap::~TileMap()
		{
			for(unsigned short a = 0; a < m_size[0]; a++)
				delete [] m_tiles[a];
			delete [] m_tiles;
		}
		void TileMap::CopyFrom(TileMap *source)
		{
			for(unsigned short a = 0; a < m_size[0]; a++)
				delete [] m_tiles[a];
			delete [] m_tiles;

			m_tileSets.clear();

			m_size = source->m_size;
			m_tileSize = source->m_tileSize;

			m_tiles = new Vector3<unsigned char> *[m_size[0]];
			for(unsigned short a = 0; a < m_size[0]; a++)
			{
				m_tiles[a] = new Vector3<unsigned char>[m_size[1]];
				for(unsigned short b = 0; b < m_size[1]; b++)
					m_tiles[a][b] = source->m_tiles[a][b];
			}

			m_tileSets.reserve(source->m_tileSets.size());
			if(source->m_tileSets.empty() == false)
			{
				TileSet **markTileSets = &source->m_tileSets.front();
				TileSet **endTileSets = markTileSets + source->m_tileSets.size();
				while(markTileSets != endTileSets)
					m_tileSets.push_back(*markTileSets++);
			}
		}
		Vector2<unsigned short> TileMap::GetSize() const
		{
			return m_size;
		}
		Vector2<unsigned short> TileMap::GetTileSize() const
		{
			return m_tileSize;
		}
		void TileMap::Render(float minX, float minY, float maxX, float maxY, float scale)
		{
			glPushMatrix();

			int _minX, _minY, _maxX, _maxY;
			_minX = (int)((minX / m_tileSize[0]) / scale) - 1;
			_maxX = (int)((maxX / m_tileSize[0]) / scale) + 1;
			_minY = (int)((minY / m_tileSize[1]) / scale) - 1;
			_maxY = (int)((maxY / m_tileSize[1]) / scale) + 1;

			if(_minX < 0)
				_minX = 0;
			if(_minY < 0)
				_minY = 0;
			if(_maxX < 0)
				_maxX = 0;
			if(_maxY < 0)
				_maxY = 0;

			glScalef(scale, scale, 1.f);
			Render(_minX, _minY, _maxX, _maxY);

			glPopMatrix();
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

			Image *lastImage = 0;
			glBegin(GL_QUADS);
			for(unsigned int a = minX; a <= maxX; a++)
				for(unsigned int b = minY; b <= maxY; b++)
				{
					Vector3<unsigned char> &tile = m_tiles[a][b];

					if(tile[0] >= m_tileSets.size())
						continue;

					TileSet *tileSet = m_tileSets[tile[0]];
					if(tileSet)
					{
						Vector2<unsigned char> tileSetSize = tileSet->GetSize();
						if(tile[1] < tileSetSize[0] && tile[2] < tileSetSize[1])
						{
							unsigned int x, y, X, Y, _x, _y, _X, _Y;
							x = a * m_tileSize[0];
							y = b * m_tileSize[1];
							X = x + m_tileSize[0];
							Y = y + m_tileSize[1];

							_x = tile[1] * m_tileSize[0];
							_y = tile[2] * m_tileSize[1];
							_X = _x + m_tileSize[0];
							_Y = _y + m_tileSize[1];

							Image *tileSetImage = tileSet->GetImage();
							Vector2<unsigned int> tileSetImageSize = tileSetImage->GetSize();

							float tx, ty, tX, tY;
							tx = ((float)_x + m_bleedLevel) / ((float)tileSetImageSize[0]);
							ty = ((float)_y + m_bleedLevel) / ((float)tileSetImageSize[1]);
							tX = ((float)_X - m_bleedLevel) / ((float)tileSetImageSize[0]);
							tY = ((float)_Y - m_bleedLevel) / ((float)tileSetImageSize[1]);

							if(lastImage != tileSetImage)
							{
								glEnd();
								tileSetImage->Bind();
								lastImage = tileSetImage;
								glBegin(GL_QUADS);
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
			glEnd();

			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}

		//- Tiles -
		Vector3<unsigned char> TileMap::GetTile(unsigned short x, unsigned short y) const
		{
			return m_tiles[x][y];
		}
		void TileMap::SetTile(unsigned short x, unsigned short y, Vector2<unsigned char> value, unsigned char tileIdx)
		{
			m_tiles[x][y][0] = tileIdx;
			m_tiles[x][y][1] = value[0];
			m_tiles[x][y][2] = value[1];
		}

		//- TileSets -
		void TileMap::AddTileSet(TileSet *tileSet)
		{
			if(HasTileSet(tileSet))
				return;
			m_tileSets.push_back(tileSet);
		}
		bool TileMap::HasTileSet(TileSet *tileSet)
		{
			return find(m_tileSets.begin(), m_tileSets.end(), tileSet) != m_tileSets.end();
		}
		unsigned char TileMap::GetTileSetIndex(TileSet *tileSet)
		{
			vector<TileSet *>::iterator it = find(m_tileSets.begin(), m_tileSets.end(), tileSet);
			if(it == m_tileSets.end())
				return 255;
			return (unsigned char)(it - m_tileSets.begin());
		}
		TileSet *TileMap::GetTileSet(unsigned char idx) const
		{
			if(idx < m_tileSets.size())
				return m_tileSets[idx];
			return 0;
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
