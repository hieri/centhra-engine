#ifndef _CE_UI_GAMEVIEW2D_H_
#define _CE_UI_GAMEVIEW2D_H_

//- Centhra Engine -
#include <CE/UI/Control.h>
#include <CE/Game2D/ZoneCamera.h>

namespace ce
{
	namespace ui
	{
		class GameView2D : public Control
		{
			game2d::ZoneCamera *m_camera;

		protected:
			virtual void DoRender();

		public:
			GameView2D(Vector2<int> position, Vector2<int> extent);

			game2d::ZoneCamera *GetCamera() const;
			void SetCamera(game2d::ZoneCamera *camera);
		};
	}
}
 
#endif
