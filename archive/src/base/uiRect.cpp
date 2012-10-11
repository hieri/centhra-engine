//- SDL2 -
#include <SDL_opengl.h>

//- Centhra Engine -
#include <CE/uiRect.h>

namespace ce
{
	uiRect::uiRect(uiInstance *ui) : uiEntity(ui)
	{
	}
	void uiRect::draw()
	{
		Vector2<int> size = getSize();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4ub(m_color[0], m_color[1], m_color[2], m_color[3]);
		glScalef((float)size[0], (float)size[1], 1.f);
		glBegin(GL_QUADS);
			glVertex2i(0, 0);
			glVertex2i(1, 0);
			glVertex2i(1, 1);
			glVertex2i(0, 1);
		glEnd();

		glDisable(GL_BLEND);
	}
	Color uiRect::getColor() const
	{
		return m_color;
	}
	void uiRect::setColor(Color color)
	{
		m_color = color;
	}
}
