//- Centhra Engine -
#include <CE/Game2D/TileSet.h>

namespace ce
{
	namespace game2d
	{
		TileSet::TileSet(Image *sourceImage, Vector2<unsigned short> tileSize)
		{
			m_sourceImage = sourceImage;
			m_tileSize = tileSize;

			Vector2<unsigned int> imageSize = sourceImage->GetSize();
			m_size = Vector2<unsigned short>(imageSize[0] / tileSize[0], imageSize[1] / tileSize[1]);
		}
		Image *TileSet::GetImage() const
		{
			return m_sourceImage;
		}
		Vector2<unsigned short> TileSet::GetTileSize() const
		{
			return m_tileSize;
		}
		Vector2<unsigned short> TileSet::GetSize() const
		{
			return m_size;
		}
	}
}
