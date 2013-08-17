//- Centhra Engine -
#include <CE/Game2D/Camera.h>

namespace ce
{
	namespace game2d
	{
		Camera *Camera::ms_current = 0;
		Camera *Camera::GetCurrent()
		{
			return ms_current;
		}
		Camera::Camera()
		{
			m_focus = 0;
		}
		void Camera::SetCurrent()
		{
			ms_current = this;
		}
		PhysicalObject *Camera::GetFocus() const
		{
			return m_focus;
		}
		void Camera::SetFocus(PhysicalObject *focus)
		{
			m_focus = focus;
		}
	}
}
