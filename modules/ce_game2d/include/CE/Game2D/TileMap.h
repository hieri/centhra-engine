#ifndef _CE_GAME2D_TILEMAP_H_
#define _CE_GAME2D_TILEMAP_H_

//- Centhra Engine -
#include <CE/Game2D/Entity.h>
#include <CE/Image.h>
#include <CE/Vector2.h>

namespace ce
{
	namespace game2d
	{
		class TileMap : public Entity
		{
			Image *m_tileSet;
			Vector2<unsigned int> m_size, m_tileSize;
			Vector2<unsigned char> **m_tiles;

		protected:
			void DoRender();

		public:
			TileMap(Vector2<unsigned int> size, Vector2<unsigned int> tileSize, Image *tileSet);
			~TileMap();

			Vector2<unsigned int> GetSize() const;
			Vector2<unsigned int> GetTileSize() const;
			Vector2<unsigned char> GetTile(unsigned int x, unsigned int y) const;
			void SetTile(unsigned int x, unsigned int y, Vector2<unsigned char> value);
			void Render(unsigned int minX, unsigned int minY, unsigned int maxX, unsigned int maxY);
		};
	}
}

#endif
