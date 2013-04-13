//- Centhra Engine -
#include <CE/UI/ColorCtrl.h>
#include <CE/Base.h>

using namespace std;

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
		ColorCtrl::ColorCtrl(Vector2<int> position, Vector2<int> extent, Color color) : Control(position, extent)
		{
			m_color = color;
		}
		void ColorCtrl::SetColor(Color color)
		{
			m_color = color;
		}
		Color ColorCtrl::GetColor() const
		{
			return m_color;
		}
		void ColorCtrl::DoRender()
		{
			Vector2<int> position = GetPosition();
			Vector2<int> extent = GetExtent();

			glPushMatrix();
				glColor4ubv(&m_color[0]);
				glBegin(GL_QUADS);
					glVertex2i(position[0], position[1]);
					glVertex2i(position[0] + extent[0], position[1]);
					glVertex2i(position[0] + extent[0], position[1] + extent[1]);
					glVertex2i(position[0], position[1] + extent[1]);
				glEnd();
				glColor4ub(255, 255, 255, 255);
			glPopMatrix();
		}
	}
}
