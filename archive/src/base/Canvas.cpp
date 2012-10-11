//- Standard Library -
#include <iostream>

#ifdef _MSC_VER
	#include <windows.h>
#else
	#include <X11/Xlib.h>
	#include <X11/cursorfont.h>
#endif

//- SDL2 -
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_syswm.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Canvas.h>
#include <CE/Cursor.h>
#include <CE/Game.h>
#include <CE/Image.h>
#include <CE/uiInstance.h>

namespace ce
{
	Canvas *Canvas::ms_current = 0;
	Canvas *Canvas::create(Vector2<int> size, unsigned int bitsPerPixel, const char *title, unsigned int flags)
	{
		Game *game = Game::getCurrent();
		assert(game != 0);

		//- Create the SDL Window -
		int sdlWindowFlags = SDL_WINDOW_OPENGL;
		if((flags & Borderless) == Borderless)
			sdlWindowFlags |= SDL_WINDOW_BORDERLESS;
		if((flags & Hidden) == Hidden)
			sdlWindowFlags |= SDL_WINDOW_HIDDEN;
		if((flags & Maximized) == Maximized)
			sdlWindowFlags |= SDL_WINDOW_MAXIMIZED;
		if((flags & Resizable) == Resizable)
			sdlWindowFlags |= SDL_WINDOW_RESIZABLE;
		if((flags & Fullscreen) == Fullscreen)
			sdlWindowFlags |= SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN;
		SDL_Window *sdlWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size[0], size[1], sdlWindowFlags);
		if(!sdlWindow)
		{
			setError("SDL_CreateWindow: %s", SDL_GetError());
			return 0;
		}

		if((flags & Minimized) == Minimized)
			SDL_MinimizeWindow(sdlWindow);

		SDL_GLContext sdlGLContext = SDL_GL_CreateContext(sdlWindow);
		if(!sdlGLContext)
		{
			setError("SDL_GL_CreateContext: %s", SDL_GetError());
			return 0;
		}

		//- Initialize OpenGL -
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetSwapInterval(1);

		Canvas *canvas = new Canvas;
		canvas->m_bitsPerPixel = bitsPerPixel;
		canvas->m_game = game;
		canvas->m_size = size;
		canvas->m_sdlGLContext = sdlGLContext;
		canvas->m_sdlWindow = sdlWindow;
		canvas->m_sdlWindowID = SDL_GetWindowID(sdlWindow);

		//- Add this Canvas to the Game -
		canvas->m_game->m_canvasMap[canvas->m_sdlWindowID] = canvas;
		canvas->m_game->m_canvasVector.insert(canvas->m_game->m_canvasVector.end(), canvas);

		canvas->setCurrent();

		return canvas;
	}
	Canvas *Canvas::getCurrent()
	{
		return ms_current;
	}
	Canvas::Canvas()
	{
		m_bitsPerPixel = 0;
		m_captureMouse = false;
		m_cursor = 0;
		m_focus = false;
		m_game = 0;
		m_lastMouseX = 0;
		m_lastMouseY = 0;
		m_sdlWindow = 0;
		m_sdlWindowID = 0;
		m_showMouse = true;
		m_syncUI = false;
		m_uiInstance = 0;
	}
	Canvas::~Canvas()
	{
		//- Remove this Canvas from the Game -
		m_game->m_canvasMap.erase(m_sdlWindowID);
		for(std::vector<Canvas *>::iterator it = m_game->m_canvasVector.begin(); it < m_game->m_canvasVector.end(); it++)
			if(*it == this)
			{
				m_game->m_canvasVector.erase(it);
				break;
			}

		//- Delete the GL Context -
		if(m_sdlGLContext)
			SDL_GL_DeleteContext((SDL_GLContext)m_sdlGLContext);

		//- Delete the SDL Window -
		if(m_sdlWindow)
			SDL_DestroyWindow((SDL_Window *)m_sdlWindow);
	}
	bool Canvas::getCaptureMouse() const
	{
		return m_captureMouse;
	}
	Cursor *Canvas::getCursor() const
	{
		return m_cursor;
	}
	bool Canvas::getFocus() const
	{
		return m_focus;
	}
	Vector2<int> Canvas::getSize() const
	{
		return m_size;
	}
	bool Canvas::getSyncUI() const
	{
		return m_syncUI;
	}
	uiInstance *Canvas::getUIInstance() const
	{
		return m_uiInstance;
	}
	void *Canvas::getWindowHandle() const
	{
		return m_sdlWindow;
	}
	void Canvas::onFocusLost()
	{
		m_focus = false;
		if(!m_showMouse)
			SDL_ShowCursor(true);
	}
	void Canvas::onFocusGained()
	{
		m_focus = true;

		SDL_ShowCursor(m_showMouse);
	}
	void Canvas::onKeyPressed(unsigned int key, unsigned short mod)
	{
		if(m_uiInstance && m_focus)
			m_uiInstance->onKeyPressed(key, mod);
	}
	void Canvas::onKeyReleased(unsigned int key, unsigned short mod)
	{
		if(m_uiInstance && m_focus)
			m_uiInstance->onKeyReleased(key, mod);
	}
	bool Canvas::onLoop()
	{
		SDL_Window *sdlWindow = (SDL_Window *)m_sdlWindow;

		//- Set this Canvas instance as the current instance -
		setCurrent();

		//- Center mouse if captured -
		if(m_captureMouse && m_focus)
		{
			SDL_WarpMouseInWindow(sdlWindow, m_size[0] / 2, m_size[1] / 2);
			SDL_PumpEvents();
		}

		if(m_uiInstance)
			m_uiInstance->render();

		//- Draw to the SDL Window -
		SDL_GL_SwapWindow(sdlWindow);
		return true;
	}
	void Canvas::onMouseButtonPressed(unsigned char button, int x, int y)
	{
		if(m_uiInstance && m_focus)
			m_uiInstance->onMouseButtonPressed(button, x, y);
	}
	void Canvas::onMouseButtonReleased(unsigned char button, int x, int y)
	{
		if(m_uiInstance && m_focus)
			m_uiInstance->onMouseButtonReleased(button, x, y);
	}
	void Canvas::onMouseEnter()
	{
		if(m_uiInstance && m_focus)
			m_uiInstance->onMouseEnter();
	}
	void Canvas::onMouseLeave()
	{
		if(m_uiInstance && m_focus)
			m_uiInstance->onMouseLeave();
	}
	void Canvas::onMouseMoved(int x, int y, int deltaX, int deltaY)
	{
		if(m_uiInstance && m_focus)
		{
			m_lastMouseX = x;
			m_lastMouseY = y;
			m_uiInstance->onMouseMoved(x, y, deltaX, deltaY);
		}
	}
	void Canvas::onMouseWheelMoved(int wheelX, int wheelY)
	{
		if(m_uiInstance && m_focus)
			m_uiInstance->onMouseWheelMoved(m_lastMouseX, m_lastMouseY, wheelX, wheelY);
	}
	void Canvas::screenshot(const char *file)
	{
		char *pixels = new char[m_size[0] * m_size[1] * 3];
		glReadPixels(0, 0, m_size[0], m_size[1], GL_RGB, GL_UNSIGNED_BYTE, pixels);

		Image *screenshot = Image::createFromPixels(pixels, 3, m_size);
		screenshot->saveToFile(file);
		delete [] pixels;
		delete screenshot;
	}
	void Canvas::setCaptureMouse(bool captureMouse)
	{
		m_captureMouse = captureMouse;

		if(m_captureMouse && m_focus)
			SDL_WarpMouseInWindow((SDL_Window *)m_sdlWindow, m_size[0] / 2, m_size[1] / 2);
	}
	void Canvas::setCurrent()
	{
		ms_current = this;
		SDL_GL_MakeCurrent((SDL_Window *)m_sdlWindow, (SDL_GLContext)m_sdlGLContext);
	}
	void Canvas::setCursor(Cursor *cursor)
	{
		if(cursor != m_cursor)
		{
			m_cursor = cursor;
			SDL_SysWMinfo sysInfo;
			SDL_VERSION(&sysInfo.version);
			if(SDL_GetWindowWMInfo((SDL_Window *)m_sdlWindow, &sysInfo) <= 0)
			{
			    error("Error: %s\n", SDL_GetError());
			}
			else
			{
#ifdef _MSC_VER
			//	print("B %i %i\n", cursor->getHandle(), sysInfo.info.win.window);
				SetCursor((HCURSOR)cursor->getHandle());
			//	SetClassLong(sysInfo.info.win.window, GCL_HCURSOR, (DWORD)cursor->getHandle());
#else
				XDefineCursor((Display *)cursor->getDisplay(), sysInfo.info.x11.window, (XID)cursor->getHandle());
				XFlush((Display *)cursor->getDisplay());
#endif
			}
		}
	}
	void Canvas::setSize(Vector2<int> size)
	{
		m_size = size;

		//- Set size of SDL Window -
		SDL_Window *sdlWindow = (SDL_Window *)m_sdlWindow;
		SDL_SetWindowSize(sdlWindow, size[0], size[1]);
	}
	void Canvas::setSyncUI(bool syncUI)
	{
		m_syncUI = syncUI;
	}
	void Canvas::setUIInstance(uiInstance *ui)
	{
		m_uiInstance = ui;

//		if(m_syncUI && m_uiInstance)
//		{
//			m_uiInstance->setWidth(m_width);
//			m_uiInstance->setHeight(m_height);
//		}
	}
	void Canvas::showMouse(bool showMouse)
	{
		m_showMouse = showMouse;
		if(m_focus)
			SDL_ShowCursor(showMouse);
	}
}
