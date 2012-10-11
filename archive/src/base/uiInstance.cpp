//- SDL2 -
#include <SDL.h>
#include <SDL_opengl.h>

//- OpenGL -
#include <GL/glu.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Canvas.h>
#include <CE/Cursor.h>
#include <CE/uiEntity.h>
#include <CE/uiInstance.h>

// TODO: Cut mouse actions when mouse leaves window.

using namespace std;

namespace ce
{
	uiEntity *g_lastEntity = 0;
	uiInstance::uiInstance(Vector2<int> size)
	{
		m_count = 0;
		m_keyFocus = 0;
		m_mouseFocus = 0;
		m_mouseDownEntity = 0;
		m_numMouseDown = 0;
		m_size = size;
	}
	uiInstance::~uiInstance()
	{
	}
	void uiInstance::add(uiEntity *entity)
	{
		if(!isMember(entity))
		{
			if(entity->getParent())
				entity->getParent()->remove(entity);
			m_objects.insert(m_objects.begin(), entity);
			m_count = m_objects.size();
		}
	}
	void uiInstance::bringToFront(uiEntity *entity)
	{
		if(isMember(entity))
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
	Color uiInstance::getClearColor() const
	{
		return m_clearColor;
	}
	unsigned int uiInstance::getCount() const
	{
		return m_count;
	}
	uiEntity *uiInstance::getEntityFromPosition(int x, int y, uiEntity *reference) const
	{
		if(reference)
		{
			Vector2<int> canvasPosition = reference->getCanvasPosition();
			Vector2<int> canvasSize = reference->getCanvasSize();
			int cx = canvasPosition[0];
			int cy = canvasPosition[1];
			int cw = canvasSize[0];
			int ch = canvasSize[1];
			
			if(x >= cx && y >= cy && x < (cx + cw) && y < (cy + ch))
			{
				for(unsigned int a = 0; a < reference->getCount(); a++)
				{
					uiEntity *search = getEntityFromPosition(x, y, reference->getObject(a));
					if(search)
						if(search->isShown())
							return search;
				}
				return reference;
			}
		}
		else
			for(unsigned int a = 0; a < getCount(); a++)
			{
				uiEntity *search = getEntityFromPosition(x, y, getObject(a));
				if(search)
					if(search->isShown())
						return search;
			}
		return 0;
	}
	uiEntity *uiInstance::getKeyFocus() const
	{
		return m_keyFocus;
	}
	uiEntity *uiInstance::getMouseFocus() const
	{
		return m_mouseFocus;
	}
	uiEntity *uiInstance::getObject(unsigned int index) const
	{
		if(index >= getCount() || index < 0)
			return 0;
		return m_objects.at(index);
	}
	Vector2<int> uiInstance::getSize() const
	{
		return m_size;
	}
	bool uiInstance::isMember(uiEntity *entity) const
	{
		for(unsigned int a = 0; a < getCount(); a++)
			if(getObject(a) == entity)
				return true;
		return false;
	}
	void uiInstance::onFocusLost()
	{
	}
	void uiInstance::onFocusGained()
	{
	}
	void uiInstance::onKeyPressed(unsigned int key, unsigned short mod)
	{
		if(m_keyFocus)
			m_keyFocus->onKeyPressed(key, mod);
	}
	void uiInstance::onKeyReleased(unsigned int key, unsigned short mod)
	{
		if(m_keyFocus)
			m_keyFocus->onKeyReleased(key, mod);
	}
	void uiInstance::onMouseButtonPressed(unsigned char button, int x, int y)
	{
		if(!m_mouseDownEntity)
		{
			if(m_mouseFocus)
				m_mouseFocus->onMouseButtonPressed(button, x, y);
			else
				m_mouseDownEntity = getEntityFromPosition(x, y);
		}
		if(m_mouseDownEntity)
			m_mouseDownEntity->onMouseButtonPressed(button, x, y);
		m_numMouseDown++;
	}
	void uiInstance::onMouseButtonReleased(unsigned char button, int x, int y)
	{
		m_numMouseDown--;
		if(m_mouseDownEntity)
		{
			m_mouseDownEntity->onMouseButtonReleased(button, x, y);
			if(!m_numMouseDown)
				m_mouseDownEntity = 0;

			uiEntity *lastEntity = g_lastEntity;
			if(g_lastEntity)
			{
				g_lastEntity->setDebugDraw(false);
				g_lastEntity = 0;
			}

			uiEntity *entity = getEntityFromPosition(x, y);
			if(entity)
			{
				entity->onMouseOver(x, y);
				entity->setDebugDraw(true);
				g_lastEntity = entity;

				if(entity != lastEntity)
				{
					if(lastEntity)
						lastEntity->onMouseLeave(x, y);
					entity->onMouseEnter(x, y);
				}
			}
		}
		else if(m_mouseFocus)
			m_mouseFocus->onMouseButtonReleased(button, x, y);
	}
	void uiInstance::onMouseEnter()
	{
	}
	void uiInstance::onMouseLeave()
	{
	}
	void uiInstance::onMouseMoved(int x, int y, int deltaX, int deltaY)
	{
		if(m_mouseDownEntity)
			m_mouseDownEntity->onMouseMoved(x, y, deltaX, deltaY);
		else if(m_mouseFocus)
			m_mouseFocus->onMouseMoved(x, y, deltaX, deltaY);
		else
		{
			uiEntity *lastEntity = g_lastEntity;
			if(g_lastEntity)
			{
				g_lastEntity->setDebugDraw(false);
				g_lastEntity = 0;
			}

			uiEntity *entity = getEntityFromPosition(x, y);
			Canvas *curCanvas = Canvas::getCurrent();
			if(entity)
			{
				entity->onMouseOver(x, y);
				entity->setDebugDraw(true);
				g_lastEntity = entity;

				if(entity != lastEntity)
				{
					if(lastEntity)
						lastEntity->onMouseLeave(x, y);
					if(curCanvas)
						curCanvas->setCursor(Cursor::getDefault(Cursor::Normal));
					entity->onMouseEnter(x, y);
				}
			}
			else if(curCanvas)
				curCanvas->setCursor(Cursor::getDefault(Cursor::Normal));
		}
	}
	void uiInstance::onMouseWheelMoved(int x, int y, int wheelX, int wheelY)
	{
		uiEntity *entity = getEntityFromPosition(x, y);
		if(entity)
			entity->onMouseWheelMoved(x, y, wheelX, wheelY);
	}
	void uiInstance::prepare2d(int x, int y, int width, int height)
	{
		Canvas *can = Canvas::getCurrent();
		Vector2<int> canExtent = can->getSize();
		float widthRatio = ((float)canExtent[0]) / ((float)m_size[0]);
		float heightRatio = ((float)canExtent[1]) / ((float)m_size[1]);

		glViewport((int)(widthRatio * (float)x), (int)(heightRatio * (float)(m_size[1] - height - y)), (int)(widthRatio * (float)width), (int)(heightRatio * (float)height));

		glDisable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D((float)x, (float)(x + width), (float)y, (float)(y + height));
	
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClear(GL_DEPTH_BUFFER_BIT);
	}
	void uiInstance::prepare3d(int x, int y, int width, int height)
	{
		Canvas *can = Canvas::getCurrent();
		Vector2<int> canExtent = can->getSize();
		float widthRatio = (float)canExtent[0] / (float)m_size[0];
		float heightRatio = (float)canExtent[1] / (float)m_size[1];

		glViewport((int)(widthRatio * (float)x), (int)(heightRatio * (float)(m_size[1] - height - y)), (int)(widthRatio * (float)width), (int)(heightRatio * (float)height));

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClearDepth(1.f);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90.f, (widthRatio * (float)width) / (heightRatio * (float)height), 0.1f, 2000.f);
	
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClear(GL_DEPTH_BUFFER_BIT);
	}
	void uiInstance::prepareUI()
	{
		Canvas *can = Canvas::getCurrent();
		Vector2<int> canExtent = can->getSize();
		float widthRatio = ((float)canExtent[0]) / ((float)m_size[0]);
		float heightRatio = ((float)canExtent[1]) / ((float)m_size[1]);

		glViewport(0, 0, canExtent[0], canExtent[1]);

		glDisable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.f, (float)m_size[0], (float)m_size[1], 0.f);
	
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClear(GL_DEPTH_BUFFER_BIT);
	}
	void uiInstance::pushToBack(uiEntity *entity)
	{
		if(isMember(entity))
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
	void uiInstance::render()
	{
		prepareUI();

		glClearColor(((float)m_clearColor[0])/255.f, ((float)m_clearColor[1])/255.f, ((float)m_clearColor[2])/255.f, ((float)m_clearColor[3])/255.f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 0, m_size[0], m_size[1]);

		for(int a = (int)(getCount()-1); a > -1; a--)
			getObject(a)->render();

		glDisable(GL_SCISSOR_TEST);
	}
	void uiInstance::remove(uiEntity *entity)
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
		}
	}
	void uiInstance::setClearColor(Color clearColor)
	{
		m_clearColor = clearColor;
	}
	void uiInstance::setKeyFocus(uiEntity *entity)
	{
		m_keyFocus = entity;
	}
	void uiInstance::setMouseFocus(uiEntity *entity)
	{
		m_mouseFocus = entity;
	}
	void uiInstance::setSize(Vector2<int> size)
	{
		m_size = size;
	}
}
