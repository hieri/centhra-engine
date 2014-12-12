//- Standard Library -
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
#include <CE/RenderPrimitives.h>
#include <CE/Base.h>

using namespace std;

namespace ce
{
	namespace ui
	{
		TextCtrl::TextCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font, const char *text, Color color) : ColorCtrl(position, extent, color),
			m_isWrapping(false)
		{
			m_type = Type_TextCtrl;
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
			m_text = formatString(format, ap);
			va_end(ap);

			if(m_isWrapping)
				ApplyWrap();
		}
		const char *TextCtrl::GetText() const
		{
			return m_text.c_str();
		}
		void TextCtrl::DoRender()
		{
			glPushMatrix();
				glColor4ub(0, 255, 0, 127);
				glScalef(m_extent[0], m_extent[1], 1.f);
				RenderSquare();
			glPopMatrix();
			glPushMatrix();
				glColor4ubv(&m_color[0]);
				m_font->DrawStringUI(m_text.c_str(), 0);
				glColor4ub(255, 255, 255, 255);
			glPopMatrix();
		}

		//- Text Wrap -
		void TextCtrl::ApplyWrap()
		{
			Vector2<int> textDimensions = m_font->StringDimensions(m_text.c_str(), 0);

//			print("Wrap: %d\t%d\n", textDimensions[0], textDimensions[1]);

			SetExtent(Vector2<int_canvas>(textDimensions[0], textDimensions[1]));
		}
		bool TextCtrl::IsWrapping() const
		{
			return m_isWrapping;
		}
		void TextCtrl::SetWrapping(bool wrapping)
		{
			if(wrapping && !m_isWrapping)
				ApplyWrap();
			m_isWrapping = wrapping;
		}
	}
}
