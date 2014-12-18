#ifndef _CE_GAME2D_APPGAME2D_H_
#define _CE_GAME2D_APPGAME2D_H_

//- Centhra Engine -
#include <CE/Vector2.h>
#include <CE/Frontend.h>
#include <CE/Mutex.h>
#include <CE/Game2D/PhysicalObject.h>
#include <CE/UI/CameraView2DCtrl.h>

namespace ce
{
	namespace game2d
	{
		class AppGame2D : public AppFrontend
		{
			//- Viewport -
		protected:
			ui::CameraView2DCtrl *m_currentViewport;
		public:
			ui::CameraView2DCtrl *GetCurrentViewport() const;
			void SetCurrentViewport(ui::CameraView2DCtrl *view);

		protected:
			Mutex m_worldMutex;
			PhysicalObject *m_referenceObject;

		public:
			AppGame2D();
			~AppGame2D();

			void LockWorldMutex();
			void UnlockWorldMutex();

			virtual Vector2<float> GetWorldPositionFromCanvasPosition(int_canvas x, int_canvas y);

			virtual bool OnStart();
			virtual void OnStopped();
		};
	}
}

#endif
