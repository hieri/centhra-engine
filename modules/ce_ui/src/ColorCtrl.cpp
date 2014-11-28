#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/UI/ColorCtrl.h>
#include <CE/Base.h>

using namespace std;

namespace ce
{
	namespace ui
	{
		ColorCtrl::ColorCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Color color) : Control(position, extent)
		{
			m_type = Type_ColorCtrl;
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
			glPushMatrix();
				glColor4ubv(&m_color[0]);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glScalef((float)m_extent[0], (float)m_extent[1], 0.f);
				glBegin(GL_QUADS);
					glVertex2i(0, 0);
					glVertex2i(1, 0);
					glVertex2i(1, 1);
					glVertex2i(0, 1);
				glEnd();
				glDisable(GL_BLEND);
				glColor4ub(255, 255, 255, 255);
			glPopMatrix();
		}
	}
}
