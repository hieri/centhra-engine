#ifndef _CE_GAME2D_TILESET_H_
#define _CE_GAME2D_TILESET_H_

//- Standard Library -
#include <string>

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
			std::string m_name;
			Image *m_sourceImage;
			Vector2<unsigned short> m_tileSize;
			Vector2<unsigned char> m_size;

		public:
			TileSet(Image *sourceImage, Vector2<unsigned short> tileSize);

			std::string GetName() const;
			void SetName(std::string name);
			Image *GetImage() const;
			Vector2<unsigned short> GetTileSize() const;
			Vector2<unsigned char> GetSize() const;

			void Render(unsigned char x, unsigned char y);
			void UIRender(unsigned char x, unsigned char y);
		};
	}
}

#endif
