#ifndef CE_CANVAS_H
#define CE_CANVAS_H

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Vector2.h>

namespace ce
{
	class uiInstance;
	class Cursor;
	class Game;
	
	/* 	Canvas Class
	 * 	- Meant to display uiInstances through a canvas created by SDL.
	 */
	class Canvas
	{
		static Canvas *ms_current;

		bool m_captureMouse, m_focus, m_showMouse, m_syncUI;
		Cursor *m_cursor;
		Game *m_game;
		int m_lastMouseX, m_lastMouseY;
		uiInstance *m_uiInstance;
		unsigned int m_bitsPerPixel, m_sdlWindowID;
		Vector2<int> m_size;
		void *m_sdlGLContext, *m_sdlWindow;

		Canvas();

	public:
		static Canvas *create(Vector2<int> size, unsigned int bitsPerPixel = 32, const char *title = "", unsigned int flags = 0);
		static Canvas *getCurrent();

		~Canvas();

		enum Flags
		{
			Borderless = 1,
			Fullscreen = 2,
			Hidden = 4,
			Maximized = 8,
			Minimized = 16,
			Resizable = 32
		};

		enum MouseButtons
		{
			Left = 1,
			Middle = 2,
			Right = 3,
			X1 = 4,
			X2 = 5
		};

		bool getCaptureMouse() const;
		Cursor *getCursor() const;
		bool getFocus() const;
		Vector2<int> getSize() const;
		bool getSyncUI() const;
		uiInstance *getUIInstance() const;
		void *getWindowHandle() const;
		void onFocusGained();
		void onFocusLost();
		void onKeyPressed(unsigned int key, unsigned short mod);
		void onKeyReleased(unsigned int key, unsigned short mod);
		bool onLoop();
		void onMouseButtonPressed(unsigned char button, int x, int y);
		void onMouseButtonReleased(unsigned char button, int x, int y);
		void onMouseEnter();
		void onMouseLeave();
		void onMouseMoved(int x, int y, int deltaX, int deltaY);
		void onMouseWheelMoved(int wheelX, int wheelY);
		void screenshot(const char *file);
		void setCaptureMouse(bool captureMouse);
		void setCurrent();
		void setCursor(Cursor *cursor);
		void setSize(Vector2<int> size);
		void setSyncUI(bool syncUI);
		void setUIInstance(uiInstance *ui);
		void showMouse(bool showMouse);
	};
}

#endif
