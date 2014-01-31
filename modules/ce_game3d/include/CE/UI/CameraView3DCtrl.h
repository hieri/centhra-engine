#ifndef _CE_UI_CAMERAVIEW3DCTRL_H_
#define _CE_UI_CAMERAVIEW3DCTRL_H_

//- Centhra Engine -
#include <CE/UI/Control.h>
#include <CE/Game2D/Camera.h>

namespace ce
{
	namespace ui
	{
		class CameraView3DCtrl : public Control
		{
		protected:
//			game2d::Camera *m_camera;

			virtual void DoRender();

		public:
			CameraView3DCtrl(Vector2<int> position, Vector2<int> extent);

//			game2d::Camera *GetCamera() const;
//			void SetCamera(game2d::Camera *camera);
		};
	}
}

#endif
