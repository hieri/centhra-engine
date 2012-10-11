//- SDL2 -
#include <SDL_opengl.h>

//- Centhra Engine -
#include <CE/Canvas.h>
#include <CE/Cursor.h>
#include <CE/uiInstance.h>
#include <CE/uiWindow.h>

// TODO: Work out how to repeat a blitted texture.  May have to resort to creating textures for each individual image in an ImageArray.

using namespace std;

namespace ce
{
	bool g_drag = false, g_resize = false;
	char g_resizeEdge = 0;
	Vector2<int> g_tempVector[3];

	uiWindow::Template::Template()
	{
		m_edgeSize = 0;
		m_font = 0;
		m_skin = 0;
		m_titleHeight = 0;
		m_titleTextSize = 0;
	}
	unsigned int uiWindow::Template::getEdgeSize() const
	{
		return m_edgeSize;
	}
	Font *uiWindow::Template::getFont() const
	{
		return m_font;
	}
	Vector2<int> uiWindow::Template::getMinimumSize() const
	{
		return m_minimumSize;
	}
	ImageArray *uiWindow::Template::getSkin() const
	{
		return m_skin;
	}
	Color uiWindow::Template::getTitleColor() const
	{
		return m_titleColor;
	}
	unsigned int uiWindow::Template::getTitleHeight() const
	{
		return m_titleHeight;
	}
	Vector2<int> uiWindow::Template::getTitlePosition() const
	{
		return m_titlePosition;
	}
	unsigned int uiWindow::Template::getTitleTextSize() const
	{
		return m_titleTextSize;
	}
	void uiWindow::Template::setEdgeSize(unsigned int edgeSize)
	{
		m_edgeSize = edgeSize;
	}
	void uiWindow::Template::setFont(Font *font)
	{
		m_font = font;
	}
	void uiWindow::Template::setMinimumSize(Vector2<int> minimumSize)
	{
		m_minimumSize = minimumSize;
	}
	void uiWindow::Template::setSkin(ImageArray *skin)
	{
		m_skin = skin;
	}
	void uiWindow::Template::setTitleColor(Color titleColor)
	{
		m_titleColor = titleColor;
	}
	void uiWindow::Template::setTitleHeight(unsigned int titleHeight)
	{
		m_titleHeight = titleHeight;
	}
	void uiWindow::Template::setTitlePosition(Vector2<int> titlePosition)
	{
		m_titlePosition = titlePosition;
	}
	void uiWindow::Template::setTitleTextSize(unsigned int titleTextSize)
	{
		m_titleTextSize = titleTextSize;
	}

	uiWindow::uiWindow(uiInstance *ui, uiWindow::Template *t) : uiEntity(ui)
	{
		m_canClose = false;
		m_canMaximize = false;
		m_canMinimize = false;
		m_canMove = false;
		m_canResize = false;
		m_template = t;
	}
	bool uiWindow::canClose() const
	{
		return m_canClose;
	}
	bool uiWindow::canMaximize() const
	{
		return m_canMaximize;
	}
	bool uiWindow::canMinimize() const
	{
		return m_canMinimize;
	}
	bool uiWindow::canMove() const
	{
		return m_canMove;
	}
	bool uiWindow::canResize() const
	{
		return m_canResize;
	}
	void uiWindow::draw()
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

		glPushMatrix();

			glColor4f(0.8f, 0.8f, 0.8f, 1.f);
			glScalef((float)size[0], (float)m_template->getTitleHeight(), 1.f);
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
			Rect<int> topLefti = skin->getRect(0, 0);
			Rect<float> topLeftf = skin->getRelativeRect(0, 0);

			Rect<int> topi = skin->getRect(1, 0);
			Rect<float> topf = skin->getRelativeRect(1, 0);
			
			Rect<int> topRighti = skin->getRect(2, 0);
			Rect<float> topRightf = skin->getRelativeRect(2, 0);
			
			Rect<int> lefti = skin->getRect(0, 1);
			Rect<float> leftf = skin->getRelativeRect(0, 1);

			Rect<float> centerf = skin->getRelativeRect(1, 1);
			
			Rect<int> righti = skin->getRect(2, 1);
			Rect<float> rightf = skin->getRelativeRect(2, 1);
			
			Rect<int> bottomLefti = skin->getRect(0, 2);
			Rect<float> bottomLeftf = skin->getRelativeRect(0, 2);

			Rect<int> bottomi = skin->getRect(1, 2);
			Rect<float> bottomf = skin->getRelativeRect(1, 2);
			
			Rect<int> bottomRighti = skin->getRect(2, 2);
			Rect<float> bottomRightf = skin->getRelativeRect(2, 2);

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
			Color titleColor = m_template->getTitleColor();
			Vector2<int> titlePosition = m_template->getTitlePosition();

			glPushMatrix();

				glColor4ub(titleColor[0], titleColor[1], titleColor[2], titleColor[3]);
				glTranslatef((float)titlePosition[0], (float)titlePosition[1], 0.f);
				font->drawText(m_title.c_str(), m_template->getTitleTextSize());

			glPopMatrix();
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}
	string uiWindow::getTitle() const
	{
		return m_title;
	}
	void uiWindow::onMouseButtonPressed(unsigned char button, int x, int y)
	{
		Vector2<int> position = getAbsolutePosition();
		Vector2<int> size = getSize();
		Vector2<int> offset = Vector2<int>(x, y) - position;
		unsigned int edgeSize = m_template->getEdgeSize();
		unsigned int edge = 0;

		if(button == Canvas::Left)
		{
			if(m_canResize)
			{
				if(offset[0] < (int)edgeSize)
					edge += 1;
				if(offset[1] < (int)edgeSize)
					edge += 2;
				if(offset[0] >= (size[0] - (int)edgeSize))
					edge += 4;
				if(offset[1] >= (size[1] - (int)edgeSize))
					edge += 8;
			}

			if(edge)
			{
				g_tempVector[0] = offset;
				g_tempVector[1] = position;
				g_tempVector[2] = size;
				g_resizeEdge = edge;
				g_resize = true;
			}
			else if((y - position[1]) < (int)m_template->getTitleHeight() && m_canMove)
			{
				g_tempVector[0] = offset;
				g_drag = true;
			}
		}

		uiEntity *parent = getParent();
		if(parent)
			parent->bringToFront(this);
	}
	void uiWindow::onMouseButtonReleased(unsigned char button, int x, int y)
	{
		if(button == Canvas::Left)
		{
			g_drag = false;
			g_resize = false;
		}
	}
	void uiWindow::onMouseMoved(int x, int y, int wheelX, int wheelY)
	{
		if(g_drag)
		{
			Vector2<int> newPosition = Vector2<int>(x, y) - g_tempVector[0];
			if(getParent())
				newPosition -= getParent()->getAbsolutePosition();
			if(newPosition[0] < 0)
				newPosition[0] = 0;
			if(newPosition[1] < 0)
				newPosition[1] = 0;

			Vector2<int> size = getSize();
			
			uiEntity *parent = getParent();
			if(parent)
			{
				Vector2<int> parentSize = parent->getSize();

				if((newPosition[0] + size[0]) > parentSize[0])
					newPosition[0] = parentSize[0] - size[0];
				if((newPosition[1] + size[1]) > parentSize[1])
					newPosition[1] = parentSize[1] - size[1];
			}
			else
			{
				Vector2<int> uiInstanceSize = getUIInstance()->getSize();

				if((newPosition[0] + size[0]) > uiInstanceSize[0])
					newPosition[0] = uiInstanceSize[0] - size[0];
				if((newPosition[1] + size[1]) > uiInstanceSize[1])
					newPosition[1] = uiInstanceSize[1] - size[1];
			}

			setPosition(newPosition);
		}
		if(g_resize)
		{
			Vector2<int> newPosition = g_tempVector[1], newSize = g_tempVector[2];
			Vector2<int> parentAbsolutePosition;
			if(getParent())
			{
				parentAbsolutePosition = getParent()->getAbsolutePosition();
				newPosition -= parentAbsolutePosition;
			}

			unsigned int advanceX = x - g_tempVector[1][0];
			unsigned int advanceY = y - g_tempVector[1][1];

			if((g_resizeEdge & 1) == 1)
			{
				newPosition[0] += advanceX;
				newSize[0] -= advanceX;
			}
			if((g_resizeEdge & 2) == 2)
			{
				newPosition[1] += advanceY;
				newSize[1] -= advanceY;
			}
			if((g_resizeEdge & 4) == 4)
				newSize[0] += advanceX - g_tempVector[2][0];
			if((g_resizeEdge & 8) == 8)
				newSize[1] += advanceY - g_tempVector[2][1];

			if(newPosition[0] < 0)
			{
				newSize[0] += newPosition[0];
				newPosition[0] = 0;
			}

			if(newPosition[1] < 0)
			{
				newSize[1] += newPosition[1];
				newPosition[1] = 0;
			}

			uiEntity *parent = getParent();
			if(parent)
			{
				Vector2<int> parentSize = parent->getSize();

				if((newSize[0] + newPosition[0]) > parentSize[0])
					newSize[0] = parentSize[0] - newPosition[0];
				if((newSize[1] + newPosition[1]) > parentSize[1])
					newSize[1] = parentSize[1] - newPosition[1];
			}
			else
			{
				Vector2<int> uiInstanceSize = getUIInstance()->getSize();

				if((newSize[0] + newPosition[0]) > uiInstanceSize[0])
					newSize[0] = uiInstanceSize[0] - newPosition[0];
				if((newSize[1] + newPosition[1]) > uiInstanceSize[1])
					newSize[1] = uiInstanceSize[1] - newPosition[1];
			}

			Vector2<int> minimumSize = m_template->getMinimumSize();
			if(newSize[0] < minimumSize[0])
			{
				newSize[0] = minimumSize[0];
				newPosition[0] = g_tempVector[1][0] - parentAbsolutePosition[0];
				if((g_resizeEdge & 1) == 1)
					newPosition[0] += g_tempVector[2][0] - minimumSize[0];
			}
			if(newSize[1] < minimumSize[1])
			{
				newSize[1] = minimumSize[1];
				newPosition[1] = g_tempVector[1][1] - parentAbsolutePosition[1];
				if((g_resizeEdge & 2) == 2)
					newPosition[1] += g_tempVector[2][1] - minimumSize[1];
			}
			if(newPosition[0] > (g_tempVector[1][0] + g_tempVector[2][0] - minimumSize[0]))
				newPosition[0] = g_tempVector[1][0] + g_tempVector[2][0] - minimumSize[0];
			if(newPosition[1] > (g_tempVector[1][1] + g_tempVector[2][1] - minimumSize[1]))
				newPosition[1] = g_tempVector[1][1] + g_tempVector[2][1] - minimumSize[1];

			setPosition(newPosition);
			setSize(newSize);
		}
	}
	void uiWindow::onMouseOver(int x, int y)
	{
		Vector2<int> position = getAbsolutePosition();
		Vector2<int> size = getSize();
		Vector2<int> offset = Vector2<int>(x, y) - position;
		unsigned int edgeSize = m_template->getEdgeSize();
		unsigned int edge = 0;

		if(m_canResize)
		{
			if(offset[0] < (int)edgeSize)
				edge += 1;
			if(offset[1] < (int)edgeSize)
				edge += 2;
			if(offset[0] >= (size[0] - (int)edgeSize))
				edge += 4;
			if(offset[1] >= (size[1] - (int)edgeSize))
				edge += 8;
		}

		if(edge)
		{
			switch(edge)
			{
			case 1:
				Canvas::getCurrent()->setCursor(Cursor::getDefault(Cursor::SizeLeft));
				break;
			case 2:
				Canvas::getCurrent()->setCursor(Cursor::getDefault(Cursor::SizeTop));
				break;
			case 4:
				Canvas::getCurrent()->setCursor(Cursor::getDefault(Cursor::SizeRight));
				break;
			case 8:
				Canvas::getCurrent()->setCursor(Cursor::getDefault(Cursor::SizeBottom));
				break;
			case 3:
				Canvas::getCurrent()->setCursor(Cursor::getDefault(Cursor::SizeTopLeft));
				break;
			case 6:
				Canvas::getCurrent()->setCursor(Cursor::getDefault(Cursor::SizeTopRight));
				break;
			case 12:
				Canvas::getCurrent()->setCursor(Cursor::getDefault(Cursor::SizeBottomRight));
				break;
			case 9:
				Canvas::getCurrent()->setCursor(Cursor::getDefault(Cursor::SizeBottomLeft));
				break;
			}
		}
		else
			Canvas::getCurrent()->setCursor(Cursor::getDefault(Cursor::Normal));

	}
	void uiWindow::setCanClose(bool canClose)
	{
		m_canClose = canClose;
	}
	void uiWindow::setCanMaximize(bool canMaximize)
	{
		m_canMaximize = canMaximize;
	}
	void uiWindow::setCanMinimize(bool canMinimize)
	{
		m_canMinimize = canMinimize;
	}
	void uiWindow::setCanMove(bool canMove)
	{
		m_canMove = canMove;
	}
	void uiWindow::setCanResize(bool canResize)
	{
		m_canResize = canResize;
	}
	void uiWindow::setTitle(string title)
	{
		m_title = title;
	}
}
