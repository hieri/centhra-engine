#ifndef _CE_GAME2D_COMPLEXTILEMAP_H_
#define _CE_GAME2D_COMPLEXTILEMAP_H_

//- Centhra Engine -
#include <CE/Game2D/TileSet.h>

//- Standard Library -
#include <vector>

namespace ce
{
	namespace game2d
	{
		class ComplexTileMap : public Entity
		{
			Vector2<unsigned short> m_size, m_tileSize;

		public:
			class Tile
			{
			public: //- TODO: Verify is this really matters -
				TileSet *m_tileSet;
				Vector2<unsigned char> m_value;

				Tile(TileSet *tileSet, Vector2<unsigned char> value);
			};

			ComplexTileMap(Vector2<unsigned short> size, Vector2<unsigned short> tileSize);
			~ComplexTileMap();

			Vector2<unsigned short> GetSize() const;
			Vector2<unsigned short> GetTileSize() const;
			Tile *GetTile(unsigned short x, unsigned short y) const;
			void SetTile(unsigned short x, unsigned short y, TileSet *tileSet, Vector2<unsigned char> value);
			void Render(unsigned short minX, unsigned short minY, unsigned short maxX, unsigned short maxY);

		private:
			Tile **m_tiles;
		};
	}
}

#endif
