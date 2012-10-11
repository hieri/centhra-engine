#ifndef CE_UIINSTANCE_H
#define CE_UIINSTANCE_H

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Color.h>
#include <CE/Vector2.h>

namespace ce
{
	class uiEntity;

	/* 	UI Instance Class
	 *	- Main container for UI objects.
	 */
	class uiInstance
	{
		Color m_clearColor;
		std::vector<uiEntity *> m_objects;
		uiEntity *m_keyFocus, *m_mouseFocus, *m_mouseDownEntity;
		unsigned int m_count, m_numMouseDown;
		Vector2<int> m_size;

	public:
		uiInstance(Vector2<int> size);
		~uiInstance();

		void add(uiEntity *entity);
		void bringToFront(uiEntity *entity);
		Color getClearColor() const;
		unsigned int getCount() const;
		uiEntity *getEntityFromPosition(int x, int y, uiEntity *reference = 0) const;
		uiEntity *getKeyFocus() const;
		uiEntity *getMouseFocus() const;
		uiEntity *getObject(unsigned int index) const;
		Vector2<int> getSize() const;
		bool isMember(uiEntity *entity) const;
		void onFocusGained();
		void onFocusLost();
		void onKeyPressed(unsigned int key, unsigned short mod);
		void onKeyReleased(unsigned int key, unsigned short mod);
		void onMouseButtonPressed(unsigned char button, int x, int y);
		void onMouseButtonReleased(unsigned char button, int x, int y);
		void onMouseEnter();
		void onMouseLeave();
		void onMouseMoved(int x, int y, int deltaX, int deltaY);
		void onMouseWheelMoved(int x, int y, int wheelX, int wheelY);
		void prepare2d(int x, int y, int width, int height);
		void prepare3d(int x, int y, int width, int height);
		void prepareUI();
		void pushToBack(uiEntity *entity);
		void remove(uiEntity *entity);
		void render();
		void setClearColor(Color clearColor);
		void setKeyFocus(uiEntity *entity);
		void setMouseFocus(uiEntity *entity);
		void setSize(Vector2<int> size);
	};
}

#endif
