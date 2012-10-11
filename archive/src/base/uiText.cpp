//- Standard Library -
#include <stdarg.h>
#include <stdio.h>

//- SDL2 -
#include <SDL_opengl.h>

//- Centhra Engine -
#include <CE/uiText.h>

using namespace std;

namespace ce
{
	uiText::uiText(uiInstance *ui) : uiEntity(ui)
	{
		m_font = Font::getDefault();
		m_textSize = 12;
	}
	void uiText::draw()
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_font->drawText(m_text.c_str(), m_textSize);

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}
	Font *uiText::getFont() const
	{
		return m_font;
	}
	string uiText::getText() const
	{
		return m_text;
	}
	unsigned int uiText::getTextSize() const
	{
		return m_textSize;
	}
	void uiText::setFont(Font *f)
	{
		m_font = f;
	}
	void uiText::setText(string text)
	{
		m_text = text;
	}
	void uiText::setText(const char *format, ...)
	{
		char text[256];
		if(format == 0)
			*text = 0;
		else
		{
			va_list	ap;
			va_start(ap, format);
			vsnprintf(text, 256, format, ap);
			va_end(ap);
		}
		m_text = string(text);
	}
	void uiText::setTextSize(unsigned int textSize)
	{
		m_textSize = textSize;
	}
}
