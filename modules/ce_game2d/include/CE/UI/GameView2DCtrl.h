#ifndef _CE_UI_GAMEVIEW2DCTRL_H_
#define _CE_UI_GAMEVIEW2DCTRL_H_

//- Centhra Engine -
#include <CE/UI/Control.h>
#include <CE/Game2D/ZoneCamera.h>

namespace ce
{
	namespace ui
	{
		class GameView2DCtrl : public Control
		{
			game2d::ZoneCamera *m_camera;
			Vector2<float> m_viewScale;

		protected:
			virtual void DoRender();

		public:
			GameView2DCtrl(Vector2<int> position, Vector2<int> extent);

			game2d::ZoneCamera *GetCamera() const;
			Vector2<float> GetViewScale() const;
			void SetCamera(game2d::ZoneCamera *camera);
			void SetViewScale(Vector2<float> viewScale);
		};
	}
}
 
#endif
