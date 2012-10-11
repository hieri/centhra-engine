//- Standard Library -
#include <stdarg.h>
#include <stdio.h>

//- SDL2 -
#include <SDL_opengl.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/uiButton.h>

// TODO: Add support for Hover-Activated states.
// TODO: Virtualize the class for custom button classes.

using namespace std;

namespace ce
{
	uiButton::Template::Template()
	{
		m_font = 0;
		m_isToggle = false;
		m_skin = 0;
		m_textSize = 0;
	}
	Font *uiButton::Template::getFont() const
	{
		return m_font;
	}
	ImageArray *uiButton::Template::getSkin() const
	{
		return m_skin;
	}
	Color uiButton::Template::getTextColor() const
	{
		return m_textColor;
	}
	Vector2<float> uiButton::Template::getTextOffset() const
	{
		return m_textOffset;
	}
	unsigned int uiButton::Template::getTextSize() const
	{
		return m_textSize;
	}
	bool uiButton::Template::isToggle() const
	{
		return m_isToggle;
	}
	void uiButton::Template::setFont(Font *font)
	{
		m_font = font;
	}
	void uiButton::Template::setSkin(ImageArray *skin)
	{
		m_skin = skin;
	}
	void uiButton::Template::setTextColor(Color textColor)
	{
		m_textColor = textColor;
	}
	void uiButton::Template::setTextOffset(Vector2<float> textOffset)
	{
		m_textOffset = textOffset;
	}
	void uiButton::Template::setTextSize(unsigned int textSize)
	{
		m_textSize = textSize;
	}
	void uiButton::Template::setToggle(bool isToggle)
	{
		m_isToggle = isToggle;
	}

	uiButton::uiButton(uiInstance *ui, uiButton::Template *t) : uiEntity(ui)
	{
		m_activated = false;
		m_state = Idle;
		m_template = t;
	}
	void uiButton::draw()
	{
		Vector2<int> size = getSize();

		glPushMatrix();

			glColor4f(1.f, 1.f, 1.f, 1.f);
			glScalef((float)size[0], (float)size[1], 1.f);
			glBegin(GL_QUADS);
				glVertex2i(0, 0);
				glVertex2i(1, 0);
				glVertex2i(1, 1);
				glVertex2i(0, 1);
			glEnd();

		glPopMatrix();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);

		ImageArray *skin = m_template->getSkin();
		if(skin)
		{
			int offsetX = 0;
			int offsetY = 0;

			switch(m_state)
			{
				case Activated:
					offsetX = 3;
					offsetY = 3;
					break;
				case Disabled:
					offsetX = 6;
					break;
				case DisabledActivated:
					offsetX = 6;
					offsetY = 3;
					break;
				case Hover:
					offsetY = 3;
					break;
				case Press:
					offsetX = 3;
					break;
			}

			Rect<int> topLefti = skin->getRect(offsetX, offsetY);
			Rect<float> topLeftf = skin->getRelativeRect(offsetX, offsetY);

			Rect<int> topi = skin->getRect(offsetX + 1, offsetY);
			Rect<float> topf = skin->getRelativeRect(offsetX + 1, offsetY);
			
			Rect<int> topRighti = skin->getRect(offsetX + 2, offsetY);
			Rect<float> topRightf = skin->getRelativeRect(offsetX + 2, offsetY);
			
			Rect<int> lefti = skin->getRect(offsetX, offsetY + 1);
			Rect<float> leftf = skin->getRelativeRect(offsetX, offsetY + 1);

			Rect<float> centerf = skin->getRelativeRect(offsetX + 1, offsetY + 1);
			
			Rect<int> righti = skin->getRect(offsetX + 2, offsetY + 1);
			Rect<float> rightf = skin->getRelativeRect(offsetX + 2, offsetY + 1);
			
			Rect<int> bottomLefti = skin->getRect(offsetX, offsetY + 2);
			Rect<float> bottomLeftf = skin->getRelativeRect(offsetX, offsetY + 2);

			Rect<int> bottomi = skin->getRect(offsetX + 1, offsetY + 2);
			Rect<float> bottomf = skin->getRelativeRect(offsetX + 1, offsetY + 2);
			
			Rect<int> bottomRighti = skin->getRect(offsetX + 2, offsetY + 2);
			Rect<float> bottomRightf = skin->getRelativeRect(offsetX + 2, offsetY + 2);

			Image *skinImage = skin->getImage();
			if(skinImage)
				skinImage->bind();

			glColor4f(1.f, 1.f, 1.f, 1.f);

			glBegin(GL_QUADS);

				//- Top -
				glTexCoord2f(topLeftf[0], topLeftf[3]); glVertex2i(0, topLefti.getHeight());
				glTexCoord2f(topLeftf[0], topLeftf[1]); glVertex2i(0, 0);
				glTexCoord2f(topLeftf[2], topLeftf[1]); glVertex2i(topLefti.getWidth(), 0);
				glTexCoord2f(topLeftf[2], topLeftf[3]); glVertex2i(topLefti.getWidth(), topLefti.getHeight());
				
				glTexCoord2f(topf[0], topf[3]); glVertex2i(topLefti.getWidth(), topi.getHeight());
				glTexCoord2f(topf[0], topf[1]); glVertex2i(topLefti.getWidth(), 0);
				glTexCoord2f(topf[2], topf[1]); glVertex2i(size[0] - topRighti.getWidth(), 0);
				glTexCoord2f(topf[2], topf[3]); glVertex2i(size[0] - topRighti.getWidth(), topi.getHeight());

				glTexCoord2f(topRightf[0], topRightf[3]); glVertex2i(size[0] - topRighti.getWidth(), topRighti.getHeight());
				glTexCoord2f(topRightf[0], topRightf[1]); glVertex2i(size[0] - topRighti.getWidth(), 0);
				glTexCoord2f(topRightf[2], topRightf[1]); glVertex2i(size[0], 0);
				glTexCoord2f(topRightf[2], topRightf[3]); glVertex2i(size[0], topRighti.getHeight());

				//- Middle -
				glTexCoord2f(leftf[0], leftf[3]); glVertex2i(0, size[1] - bottomLefti.getHeight());
				glTexCoord2f(leftf[0], leftf[1]); glVertex2i(0, topLefti.getHeight());
				glTexCoord2f(leftf[2], leftf[1]); glVertex2i(topLefti.getWidth(), topLefti.getHeight());
				glTexCoord2f(leftf[2], leftf[3]); glVertex2i(topLefti.getWidth(), size[1] - bottomLefti.getHeight());
				
				glTexCoord2f(centerf[0], centerf[3]); glVertex2i(topLefti.getWidth(), size[1] - bottomRighti.getHeight());
				glTexCoord2f(centerf[0], centerf[1]); glVertex2i(topLefti.getWidth(),  bottomRighti.getHeight());
				glTexCoord2f(centerf[2], centerf[1]); glVertex2i(size[0] - topRighti.getWidth(),  bottomRighti.getHeight());
				glTexCoord2f(centerf[2], centerf[3]); glVertex2i(size[0] - topRighti.getWidth(), size[1] - bottomRighti.getHeight());

				glTexCoord2f(rightf[0], rightf[3]); glVertex2i(size[0] - topRighti.getWidth(), size[1] - bottomRighti.getHeight());
				glTexCoord2f(rightf[0], rightf[1]); glVertex2i(size[0] - topRighti.getWidth(), bottomRighti.getHeight());
				glTexCoord2f(rightf[2], rightf[1]); glVertex2i(size[0], bottomRighti.getHeight());
				glTexCoord2f(rightf[2], rightf[3]); glVertex2i(size[0], size[1] - bottomRighti.getHeight());

				//- Bottom -
				glTexCoord2f(bottomLeftf[0], bottomLeftf[3]); glVertex2i(0, size[1]);
				glTexCoord2f(bottomLeftf[0], bottomLeftf[1]); glVertex2i(0, size[1] - bottomLefti.getHeight());
				glTexCoord2f(bottomLeftf[2], bottomLeftf[1]); glVertex2i(bottomLefti.getWidth(), size[1] - bottomLefti.getHeight());
				glTexCoord2f(bottomLeftf[2], bottomLeftf[3]); glVertex2i(bottomLefti.getWidth(), size[1]);
				
				glTexCoord2f(bottomf[0], bottomf[3]); glVertex2i(bottomLefti.getWidth(), size[1]);
				glTexCoord2f(bottomf[0], bottomf[1]); glVertex2i(bottomLefti.getWidth(), size[1] - bottomi.getHeight());
				glTexCoord2f(bottomf[2], bottomf[1]); glVertex2i(size[0] - bottomRighti.getWidth(), size[1] - bottomi.getHeight());
				glTexCoord2f(bottomf[2], bottomf[3]); glVertex2i(size[0] - bottomRighti.getWidth(), size[1]);

				glTexCoord2f(bottomRightf[0], bottomRightf[3]); glVertex2i(size[0] - bottomRighti.getWidth(), size[1]);
				glTexCoord2f(bottomRightf[0], bottomRightf[1]); glVertex2i(size[0] - bottomRighti.getWidth(), size[1] - bottomRighti.getHeight());
				glTexCoord2f(bottomRightf[2], bottomRightf[1]); glVertex2i(size[0], size[1] - bottomRighti.getHeight());
				glTexCoord2f(bottomRightf[2], bottomRightf[3]); glVertex2i(size[0], size[1]);

			glEnd();
		}

		Font *font = m_template->getFont();
		if(font)
		{
			Color textColor = m_template->getTextColor();
			Vector2<float> textOfset = m_template->getTextOffset();

			glPushMatrix();

				glColor4ub(textColor[0], textColor[1], textColor[2], textColor[3]);
		//		glTranslatef((float)textOfset[0], (float)textOfset[1], 0.f);
				font->drawText(m_text.c_str(), m_template->getTextSize());

			glPopMatrix();
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}
	string uiButton::getText() const
	{
		return m_text;
	}
	void uiButton::onMouseButtonPressed(unsigned char button, int x, int y)
	{
		m_state = Press;
	}
	void uiButton::onMouseButtonReleased(unsigned char button, int x, int y)
	{
		if(m_template->isToggle())
		{
			m_activated = !m_activated;
			if(m_activated)
				m_state = Activated;
			else
				m_state = Hover;
		}
		else
			m_state = Hover;
	}
	void uiButton::onMouseEnter(int x, int y)
	{
		m_state = Hover;
	}
	void uiButton::onMouseLeave(int x, int y)
	{
		if(m_activated)
			m_state = Activated;
		else
			m_state = Idle;
	}
	void uiButton::onMouseMoved(int x, int y, int wheelX, int wheelY)
	{
	}
	void uiButton::onMouseOver(int x, int y)
	{
	}
	void uiButton::setText(string text)
	{
		m_text = text;
	}
}
