//- Centhra Engine -
#include <CE/Game2D/TileMap.h>
#include <CE/Base.h>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

using namespace std;

//TODO: Account for camera bounds during rendering

namespace ce
{
	namespace game2d
	{
		TileMap::TileMap(Vector2<unsigned short> size, Vector2<unsigned short> tileSize)
		{
			m_size = size;
			m_tileSize = tileSize;
		}
		Vector2<unsigned short> TileMap::GetSize() const
		{
			return m_size;
		}
		Vector2<unsigned short> TileMap::GetTileSize() const
		{
			return m_tileSize;
		}
		void TileMap::Render(unsigned short minX, unsigned short minY, unsigned short maxX, unsigned short maxY)
		{
		}
	}
}
