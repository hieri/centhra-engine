//- Centhra Engine -
#include <CE/Base.h>
#include <CE/AppDisplay.h>

#include <stdlib.h>

#if CE_DISPLAY_USEXLIB
	//- Xlib -
	#include <X11/Xlib.h>

	#if CE_DISPLAY_USEXCB
		//- XCB -
		#include <X11/Xlib-xcb.h>
		#include <xcb/xcb.h>
	#endif
#endif

namespace ce
{
	AppDisplay::AppDisplay()
	{
		#if CE_DISPLAY_USEXLIB
			m_xDefaultScreen = 0;
			m_xDisplay = 0;

			#if CE_DISPLAY_USEXCB
				m_xcbConnection = 0;
			#endif
		#endif
	}
	AppDisplay::~AppDisplay()
	{
		quit();
	}
	bool AppDisplay::process()
	{
		if(!isRunning())
			return false;

		#if CE_DISPLAY_USEXCB
			xcb_generic_event_t *event = xcb_wait_for_event((xcb_connection_t *)m_xcbConnection);
			if(!event)
			{
				error("[Error] AppDisplay::process - i/o error in xcb_wait_for_event\n");
				return false;
			}

			switch(event->response_type & ~0x80)
			{
			case XCB_KEY_PRESS:
				/* Quit on key press */
				return quit();
				break;
			case XCB_EXPOSE:
				/* Handle expose event, draw and swap buffers */
				//                draw();
				//               glXSwapBuffers(display, drawable);
				break;
			default:
				break;
			}

			free(event);
		#else
			XEvent e;
			XNextEvent((Display *)m_xDisplay, &e);

			if (e.type == Expose)
			{
//				XFillRectangle((Display *)m_xDisplay, w, DefaultGC((Display *)m_xDisplay, s), 20, 20, 20, 10);
//				XDrawString((Display *)m_xDisplay, w, DefaultGC((Display *)m_xDisplay, s), 10, 50, msg, strlen(msg));
			}
			if (e.type == KeyPress)
				return quit();
		#endif

		return App::process();
	}
	bool AppDisplay::quit()
	{
		if(!isRunning())
			return false;

		#if CE_DISPLAY_USEXLIB
			#if CE_DISPLAY_USEXCB
				m_xcbConnection = 0;
			#endif
			if(m_xDisplay)
				XCloseDisplay((Display *)m_xDisplay);
			m_xDefaultScreen = 0;
			m_xDisplay = 0;
		#endif
		return App::quit();
	}
	bool AppDisplay::start()
	{
		if(isRunning())
			return false;

		setCurrent();

		#if CE_DISPLAY_USEXLIB
			Display *xDisplay = XOpenDisplay(NULL);
			if(!xDisplay)
			{
				error("[Error] AppDisplay::start - Unable to open Xlib Display.\n");
				XCloseDisplay(xDisplay);
				return false;
			}

			int xDefaultScreen = DefaultScreen(xDisplay);

			#if CE_DISPLAY_USEXCB
				xcb_connection_t *xcbConnection = XGetXCBConnection(xDisplay);
				if(!xcbConnection)
				{
					error("[Error] AppDisplay::start - Unable to get XCB connection from Xlib Display.\n");
					XCloseDisplay(xDisplay);
					return false;
				}
				XSetEventQueueOwner(xDisplay, XCBOwnsEventQueue);
			#endif
		#endif

		#if CE_DISPLAY_USEXLIB
			m_xDefaultScreen = xDefaultScreen;
			m_xDisplay = xDisplay;

			#if CE_DISPLAY_USEXCB
				m_xcbConnection = xcbConnection;
			#endif
		#endif

		return App::start();
	}

#if CE_DISPLAY_USEXLIB
	void *AppDisplay::getXDisplay() const
	{
		return m_xDisplay;
	}
	int AppDisplay::getXDefaultScreen() const
	{
		return m_xDefaultScreen;
	}
	#if CE_DISPLAY_USEXCB
		void *AppDisplay::getXCBConnection() const
		{
			return m_xcbConnection;
		}
	#endif
#endif

	bool AppDisplay::onLoop()
	{
		return true;
	}
	bool AppDisplay::onQuit()
	{
		return true;
	}
	bool AppDisplay::onStart()
	{
		return true;
	}
}
