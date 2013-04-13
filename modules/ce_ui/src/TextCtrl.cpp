//- Centhra Engine -
#include <CE/UI/TextCtrl.h>
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
		TextCtrl::TextCtrl(Vector2<int> position, Vector2<int> extent, Font *font, const char *text, Color color) : Control(position, extent)
		{
			m_color = color;
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
		void TextCtrl::SetText(const char *text)
		{
			m_text = string(text);
		}
		const char *TextCtrl::GetText() const
		{
			return m_text.c_str();
		}
		void TextCtrl::SetColor(Color color)
		{
			m_color = color;
		}
		Color TextCtrl::GetColor() const
		{
			return m_color;
		}
		void TextCtrl::DoRender()
		{
			glPushMatrix();
				glColor4ubv(&m_color[0]);
				glTranslatef(0.f, GetExtent()[1] - (float)m_font->GetCharHeight(), 0.f);
				m_font->DrawString(m_text.c_str());
				glColor4ub(255, 255, 255, 255);
			glPopMatrix();
		}
	}
}
