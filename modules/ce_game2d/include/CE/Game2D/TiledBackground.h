#ifndef _CE_GAME2D_TILEDBACKGROUND_H_
#define _CE_GAME2D_TILEDBACKGROUND_H_

//- Centhra Engine -
#include <CE/Game2D/TileMap.h>
#include <CE/UI/CameraView2DCtrl.h>

//- Standard Library -
#include <vector>

//- TODO: Generalize this -

namespace ce
{
	namespace game2d
	{
		class TiledBackground : public Entity
		{
			std::vector<std::pair<TileMap *, float> > m_tileMapVec;
			ui::CameraView2DCtrl *m_viewCtrl;

		public:
			TiledBackground(ui::CameraView2DCtrl *viewCtrl);

			void PushTileMap(TileMap *tileMap, float scale);
			void Render();
			void SetScale(Vector2<float> scale);
		};
	}
}

#endif
