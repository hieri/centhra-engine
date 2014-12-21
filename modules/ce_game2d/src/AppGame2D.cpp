//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Game2D/AppGame2D.h>

using namespace std;

namespace ce
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
		game2d::Camera *camera = game2d::Camera::GetCurrent();
		//TODO: Determine if we should start asserting
		if(m_currentViewport && camera)
		{
			Vector2<int_canvas> extent = m_currentViewport->GetExtent();
			Vector2<float> viewScale = m_currentViewport->GetViewScale();
			Vector2<float> dif((float)(x - extent[0] / 2) / viewScale[0], (float)(extent[1] / 2 - y) / viewScale[1]);
			return dif + camera->GetFocalPoint();
		}

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
		m_worldMutex.Destroy();
		AppFrontend::OnStopped();
	}
	void AppGame2D::LockWorldMutex()
	{
		m_worldMutex.Lock();
	}
	void AppGame2D::UnlockWorldMutex()
	{
		m_worldMutex.Unlock();
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
