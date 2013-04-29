//- Centhra Engine -
#include <CE/UI/GameView2D.h>
#include <CE/Game2D/Zone.h>
#include <CE/Game2D/Plane.h>
#include <CE/Base.h>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

namespace ce
{
	namespace ui
	{
		GameView2D::GameView2D(Vector2<int> position, Vector2<int> extent) : Control(position, extent)
		{
			m_camera = 0;
			m_viewScale = Vector2<float>(1.f, 1.f);
		}
		game2d::ZoneCamera *GameView2D::GetCamera() const
		{
			return m_camera;
		}
		void GameView2D::SetCamera(game2d::ZoneCamera *camera)
		{
			m_camera = camera;
		}
		void GameView2D::DoRender()
		{
			if(m_camera)
			{
				game2d::ZoneEntity *focus = m_camera->GetFocus();
				if(focus)
				{
					Vector2<float> focusPosition = focus->GetPosition();
					Vector2<float> focusExtent = focus->GetExtent();

					glPushMatrix();
						Vector2<float> half((float)m_extent[0] / 2.f, (float)m_extent[1] / 2.f);
						Vector2<float> focusHalf = focusExtent / 2.f;
						glTranslatef(half[0] - m_viewScale[0] * (focusPosition[0] + focusHalf[0]), half[1] - m_viewScale[1] * (focusPosition[1] + focusHalf[1]), 0.f);
						glScalef(m_viewScale[0], m_viewScale[1], 1.f);
						game2d::Zone *zone = focus->GetZones()[0];
						if(zone)
						{
							game2d::Plane *plane = zone->GetPlane();
							if(plane)
								plane->Render((focusPosition[0] + focusHalf[0]) - half[0] / m_viewScale[0], (focusPosition[1] + focusHalf[1]) - half[1] / m_viewScale[1], (focusPosition[0] + focusHalf[0]) + half[0] / m_viewScale[0], (focusPosition[1] + focusHalf[1]) + half[1] / m_viewScale[1]);
							else
								zone->Render();
						}
					glPopMatrix();
				}
			}
		}
		Vector2<float> GameView2D::GetViewScale() const
		{
			return m_viewScale;
		}
		void GameView2D::SetViewScale(Vector2<float> viewScale)
		{
			m_viewScale = viewScale;
		}
	}
}
