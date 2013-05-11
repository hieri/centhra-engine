//- Centhra Engine -
#include <CE/UI/TextEditCtrl.h>

#ifdef linux
	//- Linux -
	#include <unistd.h>
#endif

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
		TextEditCtrl::TextEditCtrl(Vector2<int> position, Vector2<int> extent, Font *font, const char *text, Color color) : TextCtrl(position, extent, font, text, color)
		{
		}
		void TextEditCtrl::DoRender()
		{
			glPushMatrix();
				glColor4ubv(&m_color[0]);
				glTranslatef(0.f, m_extent[1] - (float)m_font->GetCharHeight(), 0.f);
				m_font->DrawString(m_text.c_str());
				glColor4ub(255, 255, 255, 255);
			glPopMatrix();
		}
	}
}
