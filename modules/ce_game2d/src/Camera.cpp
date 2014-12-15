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
		Camera::Camera(PhysicalGroup *group, unsigned char mode) : m_focus(0), m_mode(mode), m_focusGroup(group)
		{
			if(!ms_current)
				SetCurrent();
		}
		void Camera::SetCurrent()
		{
			ms_current = this;
		}

		//- Focus -
		PhysicalObject *Camera::GetFocus() const
		{
			return m_focus;
		}
		void Camera::SetFocus(PhysicalObject *focus)
		{
			m_focus = focus;
		}
		PhysicalGroup *Camera::GetFocusGroup() const
		{
			if(m_mode == Mode_Follow)
				if(m_focus)
					return (PhysicalGroup *)m_focus->GetParentGroup();
			return m_focusGroup;
		}
		void Camera::SetFocusGroup(PhysicalGroup *focusGroup)
		{
			m_focusGroup = focusGroup;
		}

		//- Mode -
		unsigned char Camera::GetMode() const
		{
			return m_mode;
		}
		void Camera::SetMode(unsigned char mode)
		{
			m_mode = mode;
		}

		//- Position -
		Vector2<float> Camera::GetPosition() const
		{
			return m_position;
		}
		void Camera::SetPosition(Vector2<float> position)
		{
			m_position = position;
		}

		//- Focal Point -
		Vector2<float> Camera::GetFocalPoint()
		{
			if(m_mode == Mode_Follow)
				if(m_focus)
					return m_focus->GetPosition();
			return m_position;
		}
	}
}
