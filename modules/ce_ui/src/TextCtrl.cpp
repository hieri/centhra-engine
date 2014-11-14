//- Standard Library -
#include <cstdarg>
#include <cstdio>
#include <string>

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

//- Centhra Engine -
#include <CE/UI/TextCtrl.h>
#include <CE/Base.h>

using namespace std;

namespace ce
{
	namespace ui
	{
		TextCtrl::TextCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font, const char *text, Color color) : ColorCtrl(position, extent, color)
		{
			m_font = font;
			m_text = string(text);
		}
		void TextCtrl::SetFont(Font *font)
		{
			m_font = font;
		}
		Font *TextCtrl::GetFont() const
		{
			return m_font;
		}
		void TextCtrl::SetText(const char *format, ...)
		{
			va_list	ap;
			va_start(ap, format);
			m_text = compileMessage(format, ap);
			va_end(ap);
		}
		const char *TextCtrl::GetText() const
		{
			return m_text.c_str();
		}
		void TextCtrl::DoRender()
		{
			glPushMatrix();
				glColor4ubv(&m_color[0]);
				m_font->DrawStringUI(m_text.c_str());
				glColor4ub(255, 255, 255, 255);
			glPopMatrix();
		}
	}
}
