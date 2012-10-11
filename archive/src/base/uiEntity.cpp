//- SDL2 -
#include <SDL_opengl.h>

//- Centhra Engine -
#include <CE/Canvas.h>
#include <CE/uiEntity.h>
#include <CE/uiInstance.h>

using namespace std;

// TODO: Relatively scaled resizing of children upon the resizing of an entity.

namespace ce
{
	uiEntity::uiEntity(uiInstance *ui)
	{
		m_count = 0;
		m_debugDraw = false;
		m_isShown = true;
		m_isVisible = true;
		m_parent = 0;
		m_uiInstance = ui;

		ui->add(this);
	}
	uiEntity::~uiEntity()
	{
		if(m_parent)
			m_parent->remove(this);
		else
			m_uiInstance->remove(this);
	}
	void uiEntity::add(uiEntity *entity)
	{
		if(!isMember(entity))
		{
			if(entity->getParent())
				entity->getParent()->remove(entity);
			m_uiInstance->remove(entity);
			entity->m_parent = this;
			m_objects.insert(m_objects.begin(), entity);
			m_count = m_objects.size();
			
			entity->updatePosition();
		}
	}
	void uiEntity::bringToFront(uiEntity *entity)
	{
		if(isMember(entity) && getFirst() != entity)
		{
			for(vector<uiEntity *>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
				if((*it) == entity)
				{
					m_objects.erase(it);
					break;
				}
			m_objects.insert(m_objects.begin(), entity);
		}
	}
	void uiEntity::draw()
	{
	}
	Vector2<int> uiEntity::getAbsolutePosition() const
	{
		return m_absolutePosition;
	}
	Vector2<int> uiEntity::getCanvasPosition() const
	{
		return m_canvasPosition;
	}
	Vector2<int> uiEntity::getCanvasSize() const
	{
		return m_canvasSize;
	}
	unsigned int uiEntity::getCount() const
	{
		return m_count;
	}
	bool uiEntity::getDebugDraw() const
	{
		return m_debugDraw;
	}
	uiEntity *uiEntity::getFirst() const
	{
		if(!m_objects.size())
			return 0;
		return m_objects.at(0);
	}
	uiEntity *uiEntity::getLast() const
	{
		if(!m_objects.size())
			return 0;
		return m_objects.at(m_objects.size() - 1);
	}
	uiEntity *uiEntity::getObject(unsigned int index) const
	{
		if(index >= m_count || index < 0)
			return 0;
		return m_objects.at(index);
	}
	uiEntity *uiEntity::getParent() const
	{
		return m_parent;
	}
	Vector2<int> uiEntity::getPosition() const
	{
		return m_position;
	}
	Vector2<int> uiEntity::getSize() const
	{
		return m_size;
	}
	uiInstance *uiEntity::getUIInstance() const
	{
		return m_uiInstance;
	}
	bool uiEntity::hasKeyFocus() const
	{
		return m_uiInstance->getKeyFocus() == this;
	}
	bool uiEntity::hasMouseFocus() const
	{
		return m_uiInstance->getMouseFocus() == this;
	}
	bool uiEntity::isMember(uiEntity *entity) const
	{
		for(unsigned int a = 0; a < getCount(); a++)
			if(getObject(a) == entity)
				return true;
		return false;
	}
	bool uiEntity::isShown() const
	{
		return m_isShown;
	}
	bool uiEntity::isVisible() const
	{
		return m_isVisible;
	}
	void uiEntity::onKeyPressed(unsigned int key, unsigned short mod)
	{
	}
	void uiEntity::onKeyReleased(unsigned int key, unsigned short mod)
	{
	}
	void uiEntity::onMouseButtonPressed(unsigned char button, int x, int y)
	{
	}
	void uiEntity::onMouseButtonReleased(unsigned char button, int x, int y)
	{
	}
	void uiEntity::onMouseEnter(int x, int y)
	{
	}
	void uiEntity::onMouseLeave(int x, int y)
	{
	}
	void uiEntity::onMouseMoved(int x, int y, int deltaX, int deltaY)
	{
	}
	void uiEntity::onMouseOver(int x, int y)
	{
	}
	void uiEntity::onMouseWheelMoved(int x, int y, int wheelX, int wheelY)
	{
	}
	void uiEntity::pushToBack(uiEntity *entity)
	{
		if(isMember(entity) && getLast() != entity)
		{
			for(vector<uiEntity *>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
				if((*it) == entity)
				{
					m_objects.erase(it);
					break;
				}
			m_objects.insert(m_objects.end(), entity);
		}
	}
	void uiEntity::remove(uiEntity *entity)
	{
		if(isMember(entity))
		{
			for(vector<uiEntity *>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
				if((*it) == entity)
				{
					m_objects.erase(it);
					break;
				}
			m_count = m_objects.size();
			entity->m_parent = 0;
			m_uiInstance->add(entity);
			entity->updatePosition();
		}
	}
	void uiEntity::render()
	{
		if(isShown())
		{
			Vector2<int> canvasSize = Canvas::getCurrent()->getSize();

			glPushMatrix();
			glScissor(m_canvasPosition[0], canvasSize[1] - m_canvasPosition[1] - m_canvasSize[1], m_canvasSize[0], m_canvasSize[1]);
				
				glColor4f(1.f, 1.f, 1.f, 1.f);
				glTranslatef((float)m_position[0], (float)m_position[1], 0.f);
				glPushMatrix();
					draw();
				glPopMatrix();

				if(m_debugDraw)
				{
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

						glColor4f(1.f, 5.f, 0.f, 0.2f);
						glBegin(GL_QUADS);
							glVertex2i(0, 0);
							glVertex2i(m_canvasSize[0], 0);
							glVertex2i(m_canvasSize[0], m_canvasSize[1]);
							glVertex2i(0, m_canvasSize[1]);
						glEnd();

					glDisable(GL_BLEND);
				}
				
				for(int a = (int)(getCount()-1); a > -1; a--)
					getObject(a)->render();
			glPopMatrix();
		}
	}
	void uiEntity::setDebugDraw(bool debugDraw)
	{
		m_debugDraw = debugDraw;
	}
	void uiEntity::setPosition(Vector2<int> position)
	{
		m_position = position;
		updatePosition();
	}
	void uiEntity::setSize(Vector2<int> size)
	{
		m_size = size;
		updatePosition();
	}
	void uiEntity::setVisible(bool isVisible)
	{
		m_isVisible = isVisible;
		updateVisibility();
	}
	void uiEntity::updateVisibility()
	{
		m_isShown = isVisible() && m_parent ? m_parent->isShown() : true;
		
		for(unsigned int a = 0; a < getCount(); a++)
			getObject(a)->updateVisibility();
	}
	void uiEntity::updatePosition()
	{
		m_absolutePosition = m_position;

		if(m_parent)
			m_absolutePosition += m_parent->getAbsolutePosition();
		
		int cx = m_position[0];
		int cy = m_position[1];
		int cw = m_size[0];
		int ch = m_size[1];
		
		if(m_parent)
		{
			cx = m_absolutePosition[0];
			cy = m_absolutePosition[1];

			if(cx > (m_parent->m_canvasPosition[0] + m_parent->m_canvasSize[0]))
				cx = m_parent->m_canvasPosition[0] + m_parent->m_canvasSize[0];
			if(cy > (m_parent->m_canvasPosition[1] + m_parent->m_canvasSize[1]))
				cy = m_parent->m_canvasPosition[1] + m_parent->m_canvasSize[1];
			if(cx < m_parent->m_canvasPosition[0])
				cx = m_parent->m_canvasPosition[0];
			if(cy < m_parent->m_canvasPosition[1])
				cy = m_parent->m_canvasPosition[1];
		}
		
	//	if(cx > game::windowWidth)
	//		cx = game::windowWidth;
	//	if(cy > game::windowHeight)
	//		cy = game::windowWidth;
		if(cx < 0)
			cx = 0;
		if(cy < 0)
			cy = 0;
		
		if(m_parent)
		{
			cw = m_parent->m_canvasPosition[0] + m_parent->m_canvasSize[0] - cx;
			ch = m_parent->m_canvasPosition[1] + m_parent->m_canvasSize[1] - cy;
			
			if(cw > m_size[0])
				cw = m_size[0];
			if(ch > m_size[1])
				ch = m_size[1];
			if((cx + cw) > (m_absolutePosition[0] + m_size[0]))
				cw = m_absolutePosition[0] + m_size[0] - cx;
			if((cy + ch) > (m_absolutePosition[1] + m_size[1]))
				ch = m_absolutePosition[1] + m_size[1] - cy;
		}
		else
		{
			if(m_position[0] < 0)
				cx += m_position[0];
			if(m_position[1] < 0)
				cy += m_position[1];
		}
	//	if((cx + _cw) > game::windowWidth)
	//		cx = game::windowWidth - _cw;
	//	if((cy + _ch) > game::windowHeight)
	//		cy = game::windowHeight - _ch;
		if(cx < 0)
			cx = 0;
		if(cy < 0)
			cy = 0;
		if(cx < 0)
			cx = 0;
		if(cy < 0)
			cy = 0;

		m_canvasPosition = Vector2<int>(cx, cy);
		m_canvasSize = Vector2<int>(cw, ch);
		
		for(unsigned int a = 0; a < getCount(); a++)
			getObject(a)->updatePosition();
	}
}
