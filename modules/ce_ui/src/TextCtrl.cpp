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
#include <CE/Renderer.h>
#include <CE/Base.h>

using namespace std;

namespace ce
{
	namespace ui
	{
		TextCtrl::TextCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font, const char *text, Color<float> textColor, Color<float> backgroundColor)
			: ColorCtrl(position, extent, backgroundColor), m_isWrapping(false), m_textColor(textColor)
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
			m_text = formatStringList(format, ap);
			va_end(ap);

			if(m_isWrapping)
				ApplyWrap();
		}
		const char *TextCtrl::GetText() const
		{
			return m_text.c_str();
		}
		void TextCtrl::DoRender(RenderContext &context)
		{
			//TODO: Refer to previous context mvp matrix
			RenderContext currentContext = context;

			//- Render Background -
			ColorCtrl::DoRender(context);

			//- Render Text -
			{
				ShaderProgram::TexturedProgram *program = 0;
				if(context.useShaders)
					program = UseTexturedProgram();
				if(program != 0)
				{
					currentContext.mvpMatrix = context.projectionMatrix * m_absoluteMatrix;
					glUniform4fv(program->color, 1, &m_textColor[0]);
					m_font->DrawStringUI(currentContext, m_text.c_str(), 0);
					glUniform4f(program->color, 1.f, 1.f, 1.f, 1.f);
				}
				else
				{
					glColor4fv(&m_textColor[0]);
					glLoadMatrixf(&m_absoluteMatrix[0]);
					m_font->DrawStringUI(currentContext, m_text.c_str(), 0);
					glColor4ub(255, 255, 255, 255);
				}
			}
		}

		//- Text Color -
		Color<float> TextCtrl::GetTextColor() const
		{
			return m_textColor;
		}
		void TextCtrl::SetTextColor(Color<float> textColor)
		{
			m_textColor = textColor;
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
