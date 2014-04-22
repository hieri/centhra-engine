#ifndef _CE_GAME2D_SIMPLETILEMAP_H_
#define _CE_GAME2D_SIMPLETILEMAP_H_

//- Centhra Engine -
#include <CE/Game2D/TileMap.h>
#include <CE/Image.h>
#include <CE/Vector2.h>

namespace ce
{
	namespace game2d
	{
		class SimpleTileMap : public TileMap
		{
			Image *m_tileSet;
			Vector2<unsigned char> **m_tiles;

		public:
			SimpleTileMap(Vector2<unsigned short> size, Vector2<unsigned short> tileSize, Image *tileSet);
			~SimpleTileMap();

			Vector2<unsigned char> GetTile(unsigned short x, unsigned short y) const;
			void SetTile(unsigned short x, unsigned short y, Vector2<unsigned char> value);

			virtual void Render(unsigned short minX, unsigned short minY, unsigned short maxX, unsigned short maxY);
		};
	}
}

#endif
