#ifndef _CE_GAME2D_COMPLEXTILEMAP_H_
#define _CE_GAME2D_COMPLEXTILEMAP_H_

//- Centhra Engine -
#include <CE/Game2D/TileMap.h>
#include <CE/Game2D/TileSet.h>

//- Standard Library -
#include <vector>

namespace ce
{
	namespace game2d
	{
		class ComplexTileMap : public TileMap
		{
			float m_bleedLevel;

		public:
			class Tile
			{
			public: //TODO: Verify is this really matters
				TileSet *m_tileSet;
				Vector2<unsigned char> m_value;
				unsigned short m_layerIdx, m_gid;

				Tile(unsigned short layerIdx, unsigned short gid, TileSet *tileSet = 0, Vector2<unsigned char> value = Vector2<unsigned char>(0, 0));
			};

			ComplexTileMap(Vector2<unsigned short> size, Vector2<unsigned short> tileSize);
			~ComplexTileMap();

			std::vector<Tile *> *GetTiles(unsigned short x, unsigned short y) const;
			void AddTile(unsigned short layerIdx, unsigned short gid, unsigned short x, unsigned short y, TileSet *tileSet, Vector2<unsigned char> value);
			float GetBleedLevel() const;
			void SetBleedLevel(float bleedLevel);

			virtual void Render(unsigned short minX, unsigned short minY, unsigned short maxX, unsigned short maxY);

		private:
			std::vector<Tile *> **m_tiles;
		};
	}
}

#endif
