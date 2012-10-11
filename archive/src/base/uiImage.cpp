//- SDL2 -
#include <SDL_opengl.h>

//- Centhra Engine -
#include <CE/uiImage.h>

namespace ce
{
	uiImage::uiImage(uiInstance *ui) : uiRect(ui)
	{
		m_image = 0;
	}
	void uiImage::draw()
	{
		Vector2<int> size = getSize();
		Color color = getColor();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_TEXTURE_2D);
		
		glColor4ub(color[0], color[1], color[2], color[3]);
		glScalef((float)size[0], (float)size[1], 1.f);
		if(m_image)
			m_image->bind();

		glBegin(GL_QUADS);
			glTexCoord2f(0, 1); glVertex2i(0, 1);
			glTexCoord2f(0, 0); glVertex2i(0, 0);
			glTexCoord2f(1, 0); glVertex2i(1, 0);
			glTexCoord2f(1, 1); glVertex2i(1, 1);
		glEnd();

		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_TEXTURE_2D);
	}
	Image *uiImage::getImage() const
	{
		return m_image;
	}
	void uiImage::setImage(Image *img)
	{
		m_image = img;
	}
}
