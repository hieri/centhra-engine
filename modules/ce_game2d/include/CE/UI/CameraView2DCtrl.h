#ifndef _CE_GAME2D_CAMERAVIEW2DCTRL_H_
#define _CE_GAME2D_CAMERAVIEW2DCTRL_H_

//- Centhra Engine -
#include <CE/UI/Control.h>
#include <CE/Game2D/Camera.h>

namespace ce
{
	namespace ui
	{
		class CameraView2DCtrl : public Control
		{
		protected:
			game2d::Camera *m_camera;
			Vector2<float> m_viewScale;

			virtual void DoRender(RenderContext &context);
			virtual void OnSetPosition();
			virtual void OnSetExtent();

		public:
			CameraView2DCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent);

			game2d::Camera *GetCamera() const;
			Vector2<float> GetViewScale() const;
			void SetCamera(game2d::Camera *camera);
			void SetViewScale(Vector2<float> viewScale);

			//- View Matrix -
		protected:
			Matrix4x4<float> m_viewMatrix;
			bool m_vmChanged;
		public:
			void CalculateViewMatrix();
			inline Matrix4x4<float> GetViewMatrix() const
			{
				return m_viewMatrix;
			}
		};
	}
}
 
#endif
