#ifndef _CE_GAME2D_TILESET_H_
#define _CE_GAME2D_TILESET_H_

//- Centhra Engine -
#include <CE/Game2D/Entity.h>
#include <CE/Image.h>

//TODO: Verify if get functions shouldn't produce copies

namespace ce
{
	namespace game2d
	{
		class TileSet : public Entity
		{
			Image *m_sourceImage;
			Vector2<unsigned short> m_size, m_tileSize;

		public:
			TileSet(Image *sourceImage, Vector2<unsigned short> tileSize);

			Image *GetImage() const;
			Vector2<unsigned short> GetTileSize() const;
			Vector2<unsigned short> GetSize() const;
		};
	}
}

#endif
