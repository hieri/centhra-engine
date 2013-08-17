//- Centhra Engine -
#include <CE/Game2D/Camera.h>

namespace ce
{
	namespace game2d
	{
		Camera::Camera()
		{
			m_focus = 0;
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
