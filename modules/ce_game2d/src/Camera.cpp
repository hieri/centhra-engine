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
		Camera::Camera(PhysicalGroup *group, unsigned char mode) : m_focus(0), m_mode(mode), m_focusGroup(group), m_vmChanged(true)
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
			m_vmChanged = true;
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
			m_vmChanged = true;
		}

		//- Position -
		Vector2<float> Camera::GetPosition() const
		{
			return m_position;
		}
		void Camera::SetPosition(Vector2<float> position)
		{
			m_position = position;
			m_vmChanged = true;
		}

		//- Focal Point -
		Vector2<float> Camera::GetFocalPoint()
		{
			if(m_mode == Mode_Follow)
				if(m_focus)
					return m_focus->GetPosition() + m_offset;
			return m_position;
		}

		//- Offset -
		Vector2<float> Camera::GetOffset() const
		{
			return m_offset;
		}
		void Camera::SetOffset(Vector2<float> offset)
		{
			m_offset = offset;
			m_vmChanged = true;
		}

		//- View Matrix -
		bool Camera::VMChanged() const
		{
			if(m_mode == Mode_Follow)
				if(m_focus)
					if(m_focus->MVChanged())
						return true;
			return m_vmChanged;
		}
		void Camera::CalculateViewMatrix()
		{
			if(m_mode == Mode_Follow)
				if(m_focus)
					if(m_focus->MVChanged())
						m_focus->CalculateModelViewMatrix();
			Vector2<float> focalPoint = GetFocalPoint();
			m_viewMatrix = Matrix4x4<float>::BuildFromTranslation(Vector3<float>(focalPoint[0], focalPoint[1], 0));
			m_vmChanged = false;
		}
	}
}
