#ifndef _CE_GAME2D_TILEMAP_H_
#define _CE_GAME2D_TILEMAP_H_

//- Centhra Engine -
#include <CE/Game2D/Entity.h>
#include <CE/Vector2.h>

namespace ce
{
	namespace game2d
	{
		class TileMap : public Entity
		{
		protected:
			Vector2<unsigned short> m_size, m_tileSize;

		public:
			TileMap(Vector2<unsigned short> size, Vector2<unsigned short> tileSize);

			Vector2<unsigned short> GetSize() const;
			Vector2<unsigned short> GetTileSize() const;

			virtual void Render(unsigned short minX, unsigned short minY, unsigned short maxX, unsigned short maxY);
		};
	}
}

#endif
