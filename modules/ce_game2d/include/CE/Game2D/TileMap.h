#ifndef _CE_GAME2D_TILEMAP_H_
#define _CE_GAME2D_TILEMAP_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Game2D/Entity.h>
#include <CE/Game2D/TileSet.h>
#include <CE/Vector2.h>
#include <CE/Vector3.h>

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
			~TileMap();

			void CopyFrom(TileMap *source);

			Vector2<unsigned short> GetSize() const;
			Vector2<unsigned short> GetTileSize() const;

			void Render(unsigned short minX, unsigned short minY, unsigned short maxX, unsigned short maxY);
			void Render(float minX, float minY, float maxX, float maxY, float scale);

			//- Tiles -
		protected:
			Vector3<unsigned char> **m_tiles;
		public:
			Vector3<unsigned char> GetTile(unsigned short x, unsigned short y) const;
			void SetTile(unsigned short x, unsigned short y, Vector2<unsigned char> value, unsigned char tileSetIdx);

			//- TileSets -
		protected:
			std::vector<TileSet *> m_tileSets;
		public:
			void AddTileSet(TileSet *tileSet);
			bool HasTileSet(TileSet *tileSet);
			unsigned char GetTileSetIndex(TileSet *tileSet);

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
