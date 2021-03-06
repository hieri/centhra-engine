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
#include <CE/Renderer.h>
#include <CE/UI/TextEditCtrl.h>

//TODO: Add navigation via Mouse and Keyboard
//TODO: Internally handle focus for editting?

namespace ce
{
	namespace ui
	{
		TextEditCtrl::TextEditCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font, unsigned short maxSize, const char *text, Color<float> color) : TextCtrl(position, extent, font, text, color)
		{
			m_type = Type_TextEditCtrl;
			m_eventMask |= event::Mask_KeyDown | event::Mask_KeyUp;
			m_leftShift = false;
			m_rightShift = false;
			m_maxSize = maxSize;
			m_acceptsFocus = true;
			AddToFocusCtrls(this);
		}
		TextEditCtrl::~TextEditCtrl()
		{
			RemoveFromFocusCtrls(this);
		}
		void TextEditCtrl::DoRender(RenderContext &context)
		{
			TextCtrl::DoRender(context);

			//TODO: Render the cursor
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
				else if(event.key.keyCode == Key_ShiftLeft)
					m_leftShift = true;
				else if(event.key.keyCode == Key_ShiftRight)
					m_rightShift = true;
				else if(m_text.size() >= m_maxSize)
					return true;
				else if(event.key.keyCode == Key_Space)
					m_text.push_back(' ');
				else if(event.key.keyCode == Key_Return)
					m_text.push_back('\n');
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
			return Control::OnEvent(event);
		}
	}
}
