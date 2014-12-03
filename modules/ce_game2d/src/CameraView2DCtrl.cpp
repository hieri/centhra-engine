#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/UI/CameraView2DCtrl.h>
#include <CE/Game2D/PhysicalGroup.h>

namespace ce
{
	namespace ui
	{
		CameraView2DCtrl::CameraView2DCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent) : Control(position, extent)
		{
			m_camera = 0;
			m_viewScale = Vector2<float>(1.f, 1.f);
		}
		game2d::Camera *CameraView2DCtrl::GetCamera() const
		{
			return m_camera;
		}
		void CameraView2DCtrl::SetCamera(game2d::Camera *camera)
		{
			m_camera = camera;
		}
		void CameraView2DCtrl::DoRender()
		{
			if(m_camera)
			{
				game2d::PhysicalObject *focus = m_camera->GetFocus();
				if(focus)
				{
					Vector2<float> focusPosition = focus->GetPosition();
					Vector2<float> focusExtent = focus->GetExtent();

					glPushMatrix();
						glScalef(1.f, -1.f, 1.f);
						Vector2<float> half((float)m_extent[0] / 2.f, (float)m_extent[1] / 2.f);
						glTranslatef(half[0] - m_viewScale[0] * focusPosition[0], -half[1] - m_viewScale[1] * focusPosition[1], 0.f);
						glScalef(m_viewScale[0], m_viewScale[1], 1.f);

						((game2d::PhysicalGroup *)focus->GetParentGroup())->Render((focusPosition[0]) - half[0] / m_viewScale[0], (focusPosition[1]) - half[1] / m_viewScale[1], (focusPosition[0]) + half[0] / m_viewScale[0], (focusPosition[1]) + half[1] / m_viewScale[1]);
					glPopMatrix();
				}
			}
		}
		Vector2<float> CameraView2DCtrl::GetViewScale() const
		{
			return m_viewScale;
		}
		void CameraView2DCtrl::SetViewScale(Vector2<float> viewScale)
		{
			m_viewScale = viewScale;
		}
	}
}
