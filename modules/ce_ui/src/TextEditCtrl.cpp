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

//TODO: Add navigation via Mouse and Keyboard

namespace ce
{
	namespace ui
	{
		TextEditCtrl::TextEditCtrl(Vector2<int> position, Vector2<int> extent, Font *font, const char *text, Color color) : TextCtrl(position, extent, font, text, color)
		{
			m_leftShift = false;
			m_rightShift = false;
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
		bool TextEditCtrl::OnEvent(Event &event)
		{
			switch(event.type)
			{
			case event::KeyDown:
				if(event.key.keyCode == Key_Backspace)
				{
					if(m_text.size())
						m_text.erase(m_text.end() - 1);
				}
				else if(event.key.keyCode == Key_Space)
					m_text.push_back(' ');
				else if(event.key.keyCode == Key_Return)
					m_text.push_back('\n');
				else if(event.key.keyCode == Key_ShiftLeft)
					m_leftShift = true;
				else if(event.key.keyCode == Key_ShiftRight)
					m_rightShift = true;
				if(m_leftShift | m_rightShift)
				{
					if(event.key.keyCode >= Key_A && event.key.keyCode <= Key_Z)
						m_text.push_back(event.key.keyCode - ce::Key_A + 'A');
					else if(event.key.keyCode >= Key_Exclamation && event.key.keyCode <= Key_At)
						switch(event.key.keyCode)
						{
						case Key_Minus:
							m_text.push_back('_');
							break;
						case Key_Equal:
							m_text.push_back('+');
							break;
						case Key_Slash:
							m_text.push_back('?');
							break;
						case Key_Comma:
							m_text.push_back('<');
							break;
						case Key_Period:
							m_text.push_back('>');
							break;
						case Key_Apostrophe:
							m_text.push_back('\"');
							break;
						case Key_Semicolon:
							m_text.push_back(':');
							break;
						case Key_0:
							m_text.push_back(')');
							break;
						case Key_1:
							m_text.push_back('!');
							break;
						case Key_2:
							m_text.push_back('@');
							break;
						case Key_3:
							m_text.push_back('#');
							break;
						case Key_4:
							m_text.push_back('$');
							break;
						case Key_5:
							m_text.push_back('%');
							break;
						case Key_6:
							m_text.push_back('^');
							break;
						case Key_7:
							m_text.push_back('&');
							break;
						case Key_8:
							m_text.push_back('*');
							break;
						case Key_9:
							m_text.push_back('(');
							break;
						}
					else if(event.key.keyCode >= Key_BracketLeft && event.key.keyCode <= Key_Grave)
						switch(event.key.keyCode)
						{
						case Key_BracketLeft:
							m_text.push_back('{');
							break;
						case Key_BracketRight:
							m_text.push_back('}');
							break;
						case Key_Backslash:
							m_text.push_back('|');
							break;
						case Key_Grave:
							m_text.push_back('~');
							break;
						}
				}
				else
				{
					if(event.key.keyCode >= Key_BracketLeft && event.key.keyCode <= Key_Tilde)
						m_text.push_back(event.key.keyCode - Key_BracketLeft + '[');
					else if(event.key.keyCode >= Key_Exclamation && event.key.keyCode <= Key_At)
						m_text.push_back(event.key.keyCode - Key_Exclamation + '!');
				}

				break;
			case event::KeyUp:
				if(event.key.keyCode == Key_ShiftLeft)
					m_leftShift = false;
				else if(event.key.keyCode == Key_ShiftRight)
					m_rightShift = false;
				break;
			}
			return true;
		}
	}
}
