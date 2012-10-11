#ifndef CE_UIENTITY_H
#define CE_UIENTITY_H

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/uiInstance.h>
#include <CE/Vector2.h>

namespace ce
{
	/* 	UI Entity Class
	 * 	- Template class meant for emulating a UI element.
	 */
	class uiEntity
	{
		bool m_debugDraw, m_isShown, m_isVisible;
		std::vector<uiEntity *> m_objects;
		uiEntity *m_parent;
		uiInstance *m_uiInstance;
		unsigned int m_count;
		Vector2<int> m_position, m_size, m_canvasPosition, m_canvasSize, m_absolutePosition;

		void updatePosition();
		void updateVisibility();

	public:
		uiEntity(uiInstance *instance);
		~uiEntity();

		void add(uiEntity *entity);
		void bringToFront(uiEntity *entity);
		Vector2<int> getAbsolutePosition() const;
		Vector2<int> getCanvasPosition() const;
		Vector2<int> getCanvasSize() const;
		unsigned int getCount() const;
		bool getDebugDraw() const;
		uiEntity *getFirst() const;
		uiEntity *getLast() const;
		uiEntity *getObject(unsigned int index) const;
		uiEntity *getParent() const;
		Vector2<int> getPosition() const;
		Vector2<int> getSize() const;
		uiInstance *getUIInstance() const;
		bool hasKeyFocus() const;
		bool hasMouseFocus() const;
		bool isMember(uiEntity *entity) const;
		bool isShown() const;
		bool isVisible() const;
		void pushToBack(uiEntity *entity);
		void remove(uiEntity *entity);
		void setDebugDraw(bool debugDraw);
		void setPosition(Vector2<int> position);
		void setSize(Vector2<int> size);
		void setVisible(bool isVisible);

		//- User-Defined Functions -		
		virtual void onKeyPressed(unsigned int key, unsigned short mod);
		virtual void onKeyReleased(unsigned int key, unsigned short mod);
		virtual void onMouseButtonPressed(unsigned char button, int x, int y);
		virtual void onMouseButtonReleased(unsigned char button, int x, int y);
		virtual void onMouseEnter(int x, int y);
		virtual void onMouseLeave(int x, int y);
		virtual void onMouseMoved(int x, int y, int deltaX, int deltaY);
		virtual void onMouseOver(int x, int y);
		virtual void onMouseWheelMoved(int x, int y, int wheelX, int wheelY);
		virtual void render();

	protected:		
		virtual void draw();
	};
}

#endif
