#ifndef _CE_GAME2D_TILEMAP_H_
#define _CE_GAME2D_TILEMAP_H_

//- Centhra Engine -
#include <CE/Game2D/Entity.h>
#include <CE/Game2D/TileSet.h>
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
			TileMap(Vector2<unsigned short> size, Vector2<unsigned short> tileSize, TileSet *tileSet);
			~TileMap();

			Vector2<unsigned short> GetSize() const;
			Vector2<unsigned short> GetTileSize() const;

			virtual void Render(unsigned short minX, unsigned short minY, unsigned short maxX, unsigned short maxY);

			//- Tiles -
		protected:
			Vector2<unsigned char> **m_tiles;
		public:
			Vector2<unsigned char> GetTile(unsigned short x, unsigned short y) const;
			void SetTile(unsigned short x, unsigned short y, Vector2<unsigned char> value);

			//- TileSet -
		protected:
			TileSet *m_tileSet;
		public:
			TileSet *GetTileSet() const;
			void SetTileSet(TileSet *tileSet);

			//- Bleeding -
		protected:
			float m_bleedLevel;
		public:
			float GetBleedLevel() const;
			void SetBleedLevel(float bleedLevel);
		};
	}
}

#endif
