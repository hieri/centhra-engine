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

using namespace std;

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

		#if CE_FRONTEND_USEXLIB
			#if CE_FRONTEND_USEXCB
				xcb_generic_event_t *xcbEvent;
				xcb_window_t xcbWindow;
				while((xcbEvent = xcb_poll_for_event((xcb_connection_t *)m_xcbConnection)))
				{
					Event event;
					event.base.canvas = 0;

					switch(xcbEvent->response_type & ~0x80)
					{
					case XCB_DESTROY_NOTIFY:
					case XCB_UNMAP_NOTIFY:
						return quit();
					case XCB_KEY_PRESS:
					{
						xcb_button_press_event_t *cast = (xcb_button_press_event_t *)xcbEvent;
						event.type = KeyDown;
						xcbWindow = cast->event;
						if(m_canvasMap.count(xcbWindow))
							event.base.canvas = m_canvasMap[xcbWindow];
						event.key.keyCode = cast->detail;
						event.key.state = cast->state;
						onEvent(event);
						break;
					}
					case XCB_KEY_RELEASE:
					{
						xcb_key_release_event_t *cast = (xcb_key_release_event_t *)xcbEvent;
						event.type = KeyUp;
						xcbWindow = cast->event;
						if(m_canvasMap.count(xcbWindow))
							event.base.canvas = m_canvasMap[xcbWindow];
						event.key.keyCode = cast->detail;
						event.key.state = cast->state;
						onEvent(event);
						break;
					}
					case XCB_BUTTON_PRESS:
					{
						xcb_button_release_event_t *cast = (xcb_button_release_event_t *)xcbEvent;
						event.type = MouseButtonDown;
						xcbWindow = cast->event;
						if(m_canvasMap.count(xcbWindow))
							event.base.canvas = m_canvasMap[xcbWindow];
						event.mouseButton.button = cast->detail;
						event.mouseButton.state = cast->state;
						event.mouseButton.x = cast->event_x;
						event.mouseButton.y = cast->event_y;
						onEvent(event);
						break;
					}
					case XCB_BUTTON_RELEASE:
					{
						xcb_button_release_event_t *cast = (xcb_button_release_event_t *)xcbEvent;
						event.type = MouseButtonUp;
						xcbWindow = cast->event;
						if(m_canvasMap.count(xcbWindow))
							event.base.canvas = m_canvasMap[xcbWindow];
						event.mouseButton.button = cast->detail;
						event.mouseButton.state = cast->state;
						event.mouseButton.x = cast->event_x;
						event.mouseButton.y = cast->event_y;
						onEvent(event);
						break;
					}
					case XCB_MOTION_NOTIFY:
					{
						xcb_motion_notify_event_t *cast = (xcb_motion_notify_event_t *)xcbEvent;
						event.type = MouseMotion;
						xcbWindow = cast->event;
						if(m_canvasMap.count(xcbWindow))
							event.base.canvas = m_canvasMap[xcbWindow];
						event.mouseMotion.x = cast->event_x;
						event.mouseMotion.y = cast->event_y;
						onEvent(event);
						break;
					}
					default:
						break;
					}
					free(xcbEvent);
				}
			#else
				while(XPending((Display *)m_xDisplay))
				{
					XEvent xEvent;
					XNextEvent((Display *)m_xDisplay, &xEvent);

					Window xWindow = xEvent.xany.window;

					Event event;
					event.base.canvas = 0;
					if(m_canvasMap.count(xWindow))
						event.base.canvas = m_canvasMap[xWindow];

					switch(xEvent.type)
					{
					case DestroyNotify:
					case UnmapNotify:
						return quit();
					case KeyPress:
						event.type = KeyDown;
						event.key.keyCode = xEvent.xkey.keycode;
						event.key.state = xEvent.xkey.state;
						onEvent(event);
						break;
					case KeyRelease:
						event.type = KeyUp;
						event.key.keyCode = xEvent.xkey.keycode;
						event.key.state = xEvent.xkey.state;
						onEvent(event);
						break;;
					case ButtonPress:
						event.mouseButton.type = MouseButtonDown;
						event.mouseButton.button = xEvent.xbutton.button;
						event.mouseButton.state = xEvent.xbutton.state;
						event.mouseButton.x = xEvent.xbutton.x;
						event.mouseButton.y = xEvent.xbutton.y;
						onEvent(event);
						break;
					case ButtonRelease:
						event.mouseButton.type = MouseButtonUp;
						event.mouseButton.button = xEvent.xbutton.button;
						event.mouseButton.state = xEvent.xbutton.state;
						event.mouseButton.x = xEvent.xbutton.x;
						event.mouseButton.y = xEvent.xbutton.y;
						onEvent(event);
						break;
					case MotionNotify:
						event.type = MouseMotion;
						event.mouseMotion.x = xEvent.xmotion.x;
						event.mouseMotion.y = xEvent.xmotion.y;
						onEvent(event);
						break;
					}
				}
			#endif
		#endif

		#if CE_FRONTEND_USEWIN
			MSG wMsg;
			while(PeekMessage(&wMsg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&wMsg);
				DispatchMessage(&wMsg);
			}
		#endif

		#if CE_FRONTEND_USEXLIB
			map<int, Canvas *>::iterator it;
		#endif
		#if CE_FRONTEND_USEWIN
			map<void *, Canvas *>::iterator it;
		#endif
		for(it = m_canvasMap.begin(); it != m_canvasMap.end(); it++)
			it->second->render();

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
		Canvas *canvas = event.base.canvas;
		return (canvas) ? canvas->onEvent(event) : true;
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
