//- Standard Library -
#include <stdarg.h>
#include <stdio.h>
#include <string>

#ifdef linux
	//- Linux -
	#include <unistd.h>
#endif

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>

	#if _MSC_VER >= 1400
		#define snprintf _snprintf_s
		#define vsnprintf _vsnprintf_s
	#endif
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/UI/TextCtrl.h>
#include <CE/Base.h>

using namespace std;

//TODO: Move this into a support snippet file, or Base.h
string compileMessage(const char *format, va_list ap)
{
	char text[4096];
	vsnprintf(text, 4096, format, ap);
	return string(text);
}

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
				glTranslatef(0.f, m_extent[1] - (float)m_font->GetCharHeight(), 0.f);
				m_font->DrawString(m_text.c_str());
				glColor4ub(255, 255, 255, 255);
			glPopMatrix();
		}
	}
}
