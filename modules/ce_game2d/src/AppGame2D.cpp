//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Game2D/AppGame2D.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		AppGame2D::AppGame2D() : AppFrontend()
		{
			m_referenceObject = 0;
			m_currentViewport = 0;
		}
		AppGame2D::~AppGame2D()
		{
		}
		Vector2<float> AppGame2D::GetWorldPositionFromCanvasPosition(int_canvas x, int_canvas y)
		{
			return Vector2<float>(0.f, 0.f);
		}
		bool AppGame2D::OnStart()
		{
			bool parentStart = AppFrontend::OnStart();
			if(!parentStart)
				return false;
			m_worldMutex.Init();
			return true;
		}
		void AppGame2D::OnStopped()
		{
			AppFrontend::OnStopped();
			m_worldMutex.Destroy();
		}
		void AppGame2D::LockWorldMutex()
		{
			m_worldMutex.Lock();
		}
		void AppGame2D::UnlockWorldMutex()
		{
			m_worldMutex.Unlock();
		}
		PhysicalObject *AppGame2D::GetReferenceObject() const
		{
			return m_referenceObject;
		}
		void AppGame2D::SetReferenceObject(PhysicalObject *object)
		{
			m_referenceObject = object;
		}

		//- Viewport -
		ui::CameraView2DCtrl *AppGame2D::GetCurrentViewport() const
		{
			return m_currentViewport;
		}
		void AppGame2D::SetCurrentViewport(ui::CameraView2DCtrl *view)
		{
			m_currentViewport = view;
		}
	}
}
