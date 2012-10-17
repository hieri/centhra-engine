//- Standard Library -
#include <stdlib.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/AppFrontend.h>
#include <CE/Canvas.h>

#if CE_FRONTEND_USEXLIB
	//- Xlib -
	#include <X11/Xlib.h>

	#if CE_FRONTEND_USEXCB
		//- XCB -
		#include <X11/Xlib-xcb.h>
		#include <xcb/xcb.h>
	#endif
#endif

#ifdef _WIN32
	#include <windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

namespace ce
{
	AppFrontend::AppFrontend()
	{
		#if CE_FRONTEND_USEXLIB
			m_xDefaultScreen = 0;
			m_xDisplay = 0;

			#if CE_FRONTEND_USEXCB
				m_xcbConnection = 0;
			#endif
		#endif

		#if CE_FRONTEND_USEWIN
			m_hInstance = 0;
		#endif
	}
	AppFrontend::~AppFrontend()
	{
		quit();
	}
	bool AppFrontend::process()
	{
		if(!isRunning())
			return false;

		#if CE_FRONTEND_USEXCB
		/*
			xcb_generic_event_t *xcbEvent = xcb_wait_for_event((xcb_connection_t *)m_xcbConnection);
			if(!xcbEvent)
			{
				error("[Error] AppFrontend::process - i/o error in xcb_wait_for_event\n");
				return false;
			}
		*/
			xcb_generic_event_t *xcbEvent;
			while(xcbEvent = xcb_poll_for_event((xcb_connection_t *)m_xcbConnection))
			{
				Event event;
				switch(xcbEvent->response_type & ~0x80)
				{
				case XCB_EXPOSE:
					/* Handle expose event, draw and swap buffers */
					//                draw();
					//               glXSwapBuffers(display, drawable);
					break;
				case XCB_KEY_PRESS:
					event.key.type = KeyDown;
					event.key.keyCode = ((xcb_button_press_event_t *)xcbEvent)->detail;
					event.key.state = ((xcb_button_press_event_t *)xcbEvent)->state;
					onEvent(event);
					break;
				case XCB_KEY_RELEASE:
					event.key.type = KeyUp;
					event.key.keyCode = ((xcb_button_press_event_t *)xcbEvent)->detail;
					event.key.state = ((xcb_button_press_event_t *)xcbEvent)->state;
					onEvent(event);
					break;
				}
				free(xcbEvent);
			}
		#endif

		#if CE_FRONTEND_USEXLIB
			while(XPending((Display *)m_xDisplay))
			{
				XEvent xEvent;
				XNextEvent((Display *)m_xDisplay, &xEvent);
				Event event;

				switch(xEvent.type)
				{
					case Expose:
						break;
					case KeyPress:
						event.key.type = KeyDown;
						event.key.keyCode = xEvent.xkey.keycode;
						event.key.state = xEvent.xkey.state;
						onEvent(event);
						break;
					case KeyRelease:
						event.key.type = KeyUp;
						event.key.keyCode = xEvent.xkey.keycode;
						event.key.state = xEvent.xkey.state;
						onEvent(event);
						break;
				}
	//			if (e.type == Expose)
	//			{
	//				XFillRectangle((Display *)m_xDisplay, w, DefaultGC((Display *)m_xDisplay, s), 20, 20, 20, 10);
	//				XDrawString((Display *)m_xDisplay, w, DefaultGC((Display *)m_xDisplay, s), 10, 50, msg, strlen(msg));
	//			}
	//			if (e.type == KeyPress)
	//				return quit();
			}
		#endif

		#if CE_FRONTEND_USEWIN
			MSG wMsg;
			while(PeekMessage(&wMsg, NULL, 0, 0, PM_REMOVE))
			{
				if(m_canvasMap.count(wMsg.hwnd))
				{
					Canvas *canvas = m_canvasMap[wMsg.hwnd];
					Event event;

					switch(wMsg.message)
					{
					case WM_DESTROY:
						PostQuitMessage(0);
						return quit();
					case WM_KEYDOWN:
						event.key.type = KeyDown;
						event.key.keyCode = wMsg.wParam;
						event.key.state = 1;
						onEvent(event);
						break;
					case WM_KEYUP:
						event.key.type = KeyUp;
						event.key.keyCode = wMsg.wParam;
						event.key.state = 0;
						onEvent(event);
						break;
					}
				}
				else
				{
					//- TODO: Pop message back in queue for other applications, if necessary. -
				}

				TranslateMessage(&wMsg);
				DispatchMessage(&wMsg);
			}
		#endif

		return App::process();
	}
	bool AppFrontend::quit()
	{
		if(!isRunning())
			return false;

		#if CE_FRONTEND_USEXLIB
			#if CE_FRONTEND_USEXCB
				m_xcbConnection = 0;
			#endif
			if(m_xDisplay)
				XCloseDisplay((Display *)m_xDisplay);
			m_xDefaultScreen = 0;
			m_xDisplay = 0;
		#endif

		#if CE_FRONTEND_USEWIN
			m_hInstance = 0;
		#endif

		return App::quit();
	}
	bool AppFrontend::start()
	{
		if(isRunning())
			return false;

		setCurrent();

		#if CE_FRONTEND_USEXLIB
			Display *xDisplay = XOpenDisplay(NULL);
			if(!xDisplay)
			{
				error("[Error] AppFrontend::start - Unable to open Xlib Display.\n");
				XCloseDisplay(xDisplay);
				return false;
			}

			int xDefaultScreen = DefaultScreen(xDisplay);

			#if CE_FRONTEND_USEXCB
				xcb_connection_t *xcbConnection = XGetXCBConnection(xDisplay);
				if(!xcbConnection)
				{
					error("[Error] AppFrontend::start - Unable to get XCB connection from Xlib Display.\n");
					XCloseDisplay(xDisplay);
					return false;
				}
				XSetEventQueueOwner(xDisplay, XCBOwnsEventQueue);
			#endif
		#endif

		#if CE_FRONTEND_USEXLIB
			m_xDefaultScreen = xDefaultScreen;
			m_xDisplay = xDisplay;

			#if CE_FRONTEND_USEXCB
				m_xcbConnection = xcbConnection;
			#endif
		#endif

		#if CE_FRONTEND_USEWIN
			m_hInstance = GetModuleHandle(NULL);
		#endif

		return App::start();
	}

#if CE_FRONTEND_USEXLIB
	void *AppFrontend::getXDisplay() const
	{
		return m_xDisplay;
	}
	int AppFrontend::getXDefaultScreen() const
	{
		return m_xDefaultScreen;
	}
	#if CE_FRONTEND_USEXCB
		void *AppFrontend::getXCBConnection() const
		{
			return m_xcbConnection;
		}
	#endif
#endif

#if CE_FRONTEND_USEWIN
	void *AppFrontend::getHInstance() const
	{
		return m_hInstance;
	}
#endif

	bool AppFrontend::onEvent(Event &event)
	{
		switch(event.type)
		{
			case KeyDown:
				print("Down: %i %i\n", event.key.keyCode, event.key.state);
				break;
			case KeyUp:
				print("Up: %i %i\n", event.key.keyCode, event.key.state);
				break;
		}

		return true;
	}
	bool AppFrontend::onLoop()
	{
		return true;
	}
	bool AppFrontend::onQuit()
	{
		return true;
	}
	bool AppFrontend::onStart()
	{
		return true;
	}
}
