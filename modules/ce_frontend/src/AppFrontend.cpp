//- Standard Library -
#include <stdlib.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/AppFrontend.h>
#include <CE/Canvas.h>
#include <CE/Keyboard.h>

#if CE_FRONTEND_USEXLIB
	//- Xlib -
	#include <X11/Xatom.h>
	#include <X11/Xlib.h>

	#if CE_FRONTEND_USEXCB
		//- XCB -
		#include <X11/Xlib-xcb.h>
		#include <xcb/xcb.h>
	#endif
#endif

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

using namespace std;

//- TODO: properly handle X11/XCB quit events -

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
		Stop(true);
	}
	bool AppFrontend::Process()
	{
		if(!IsRunning())
			return false;

		#if CE_FRONTEND_USEXLIB
			#if CE_FRONTEND_USEXCB
				xcb_generic_event_t *xcbEvent;
				xcb_window_t xcbWindow;
				while((xcbEvent = xcb_poll_for_event((xcb_connection_t *)m_xcbConnection)))
				{
					Event event;
					event.base.timeMS = GetRunTimeMS();
					event.base.canvas = 0;

					switch(xcbEvent->response_type & ~0x80)
					{
					case XCB_CLIENT_MESSAGE:
						if((*(xcb_client_message_event_t*)xcbEvent).data.data32[0] == m_wmDeleteMessage)
							return Stop();
						break;
					case XCB_DESTROY_NOTIFY:
					case XCB_UNMAP_NOTIFY:
						return Stop(true);
					case XCB_KEY_PRESS:
					{
						xcb_button_press_event_t *cast = (xcb_button_press_event_t *)xcbEvent;
						event.type = event::KeyDown;
						xcbWindow = cast->event;
						if(m_canvasMap.count(xcbWindow))
							event.base.canvas = m_canvasMap[xcbWindow];
						event.key.scanCode = NativeScanCodeToScanCode(cast->detail);
						event.key.keyCode = ScanCodeToKeyCode(event.key.scanCode);
						event.key.state = cast->state;
						OnEvent(event);
						break;
					}
					case XCB_KEY_RELEASE:
					{
						xcb_key_release_event_t *cast = (xcb_key_release_event_t *)xcbEvent;
						event.type = event::KeyUp;
						xcbWindow = cast->event;
						if(m_canvasMap.count(xcbWindow))
							event.base.canvas = m_canvasMap[xcbWindow];
						event.key.scanCode = NativeScanCodeToScanCode(cast->detail);
						event.key.keyCode = ScanCodeToKeyCode(event.key.scanCode);
						event.key.state = cast->state;
						OnEvent(event);
						break;
					}
					case XCB_BUTTON_PRESS:
					{
						xcb_button_release_event_t *cast = (xcb_button_release_event_t *)xcbEvent;
						event.type = event::MouseButtonDown;
						xcbWindow = cast->event;
						if(m_canvasMap.count(xcbWindow))
							event.base.canvas = m_canvasMap[xcbWindow];
						switch(cast->detail)
						{
						case 1:
							event.mouseButton.button = event::MouseButtonLeft;
							break;
						case 2:
							event.mouseButton.button = event::MouseButtonMiddle;
							break;
						case 3:
							event.mouseButton.button = event::MouseButtonRight;
							break;
						default:
							event.mouseButton.button = event::Unknown;
						}
						event.mouseButton.state = cast->state;
						event.mouseButton.x = cast->event_x;
						event.mouseButton.y = cast->event_y;
						OnEvent(event);
						break;
					}
					case XCB_BUTTON_RELEASE:
					{
						xcb_button_release_event_t *cast = (xcb_button_release_event_t *)xcbEvent;
						event.type = event::MouseButtonUp;
						xcbWindow = cast->event;
						if(m_canvasMap.count(xcbWindow))
							event.base.canvas = m_canvasMap[xcbWindow];
						switch(cast->detail)
						{
						case 1:
							event.mouseButton.button = event::MouseButtonLeft;
							break;
						case 2:
							event.mouseButton.button = event::MouseButtonMiddle;
							break;
						case 3:
							event.mouseButton.button = event::MouseButtonRight;
							break;
						default:
							event.mouseButton.button = event::Unknown;
						}
						event.mouseButton.state = cast->state;
						event.mouseButton.x = cast->event_x;
						event.mouseButton.y = cast->event_y;
						OnEvent(event);
						break;
					}
					case XCB_MOTION_NOTIFY:
					{
						xcb_motion_notify_event_t *cast = (xcb_motion_notify_event_t *)xcbEvent;
						event.type = event::MouseMotion;
						xcbWindow = cast->event;
						if(m_canvasMap.count(xcbWindow))
							event.base.canvas = m_canvasMap[xcbWindow];
						event.mouseMotion.x = cast->event_x;
						event.mouseMotion.y = cast->event_y;
						OnEvent(event);
						break;
					}
					case XCB_CONFIGURE_NOTIFY:
					{
						xcb_configure_notify_event_t *cast = (xcb_configure_notify_event_t *)xcbEvent;
						xcbWindow = cast->event;
						if(m_canvasMap.count(xcbWindow))
							event.base.canvas = m_canvasMap[xcbWindow];

						if(cast->width != event.base.canvas->GetWidth() || cast->height != event.base.canvas->GetHeight())
						{
							if(!event.base.canvas->IsFullscreen())
								event.base.canvas->SetWindowedExtent(cast->width, cast->height);
							event.base.canvas->UpdateViewport(cast->width, cast->height);
							event.type = event::WindowResize;
							event.windowResize.width = cast->width;
							event.windowResize.height = cast->height;
							OnEvent(event);
						}

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
					event.base.timeMS = GetRunTimeMS();
					event.base.canvas = 0;
					if(m_canvasMap.count(xWindow))
						event.base.canvas = m_canvasMap[xWindow];

					switch(xEvent.type)
					{
					case ClientMessage:
						if(xEvent.xclient.data.l[0] == m_wmDeleteMessage)
							return Stop();
						break;
					case DestroyNotify:
					case UnmapNotify:
						return Stop(true);
					case KeyPress:
						event.type = event::KeyDown;
						event.key.scanCode = NativeScanCodeToScanCode(xEvent.xkey.keycode);
						event.key.keyCode = ScanCodeToKeyCode(event.key.scanCode);
						event.key.state = xEvent.xkey.state;
						OnEvent(event);
						break;
					case KeyRelease:
						event.type = event::KeyUp;
						event.key.scanCode = NativeScanCodeToScanCode(xEvent.xkey.keycode);
						event.key.keyCode = ScanCodeToKeyCode(event.key.scanCode);
						event.key.state = xEvent.xkey.state;
						OnEvent(event);
						break;;
					case ButtonPress:
						event.mouseButton.type = event::MouseButtonDown;
						switch(xEvent.xbutton.button)
						{
						case 1:
							event.mouseButton.button = event::MouseButtonLeft;
							break;
						case 2:
							event.mouseButton.button = event::MouseButtonMiddle;
							break;
						case 3:
							event.mouseButton.button = event::MouseButtonRight;
							break;
						default:
							event.mouseButton.button = event::Unknown;
						}
						event.mouseButton.state = xEvent.xbutton.state;
						event.mouseButton.x = xEvent.xbutton.x;
						event.mouseButton.y = xEvent.xbutton.y;
						OnEvent(event);
						break;
					case ButtonRelease:
						event.mouseButton.type = event::MouseButtonUp;
						switch(xEvent.xbutton.button)
						{
						case 1:
							event.mouseButton.button = event::MouseButtonLeft;
							break;
						case 2:
							event.mouseButton.button = event::MouseButtonMiddle;
							break;
						case 3:
							event.mouseButton.button = event::MouseButtonRight;
							break;
						default:
							event.mouseButton.button = event::Unknown;
						}
						event.mouseButton.state = xEvent.xbutton.state;
						event.mouseButton.x = xEvent.xbutton.x;
						event.mouseButton.y = xEvent.xbutton.y;
						OnEvent(event);
						break;
					case MotionNotify:
						event.type = event::MouseMotion;
						event.mouseMotion.x = xEvent.xmotion.x;
						event.mouseMotion.y = xEvent.xmotion.y;
						OnEvent(event);
						break;
					case ConfigureNotify:
						if(xEvent.xconfigure.width != event.base.canvas->GetWidth() || xEvent.xconfigure.height != event.base.canvas->GetHeight())
						{
							if(!event.base.canvas->IsFullscreen())
								event.base.canvas->SetWindowedExtent(xEvent.xconfigure.width, xEvent.xconfigure.height);
							event.base.canvas->UpdateViewport(xEvent.xconfigure.width, xEvent.xconfigure.height);
							event.type = event::WindowResize;
							event.windowResize.width = xEvent.xconfigure.width;
							event.windowResize.height = xEvent.xconfigure.height;
							OnEvent(event);
						}
						break;
					case FocusOut:
						if(event.base.canvas->IsFullscreen())
							event.base.canvas->SetFullscreen(false);
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
			it->second->Render();

		return App::Process();
	}
	bool AppFrontend::Start()
	{
		if(IsRunning())
			return false;

		SetCurrent();

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

				xcb_intern_atom_cookie_t cookie = xcb_intern_atom(xcbConnection, 0, 16, "WM_DELETE_WINDOW");
				xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(xcbConnection, cookie, 0);
				xcb_atom_t wmDeleteMessage = reply->atom;
			#else
				Atom wmDeleteMessage = XInternAtom(xDisplay, "WM_DELETE_WINDOW", False);
			#endif
		#endif

		#if CE_FRONTEND_USEXLIB
			m_wmDeleteMessage = wmDeleteMessage;
			m_xDefaultScreen = xDefaultScreen;
			m_xDisplay = xDisplay;

			#if CE_FRONTEND_USEXCB
				m_xcbConnection = xcbConnection;
			#endif
		#endif

		#if CE_FRONTEND_USEWIN
			m_hInstance = GetModuleHandle(NULL);
		#endif

		return App::Start();
	}
	bool AppFrontend::Stop(bool force)
	{
		if(!IsRunning())
			return false;

		bool isValid = OnStop(force) || force;
		if(isValid)
		{
			m_isRunning = false;
			OnStopped();

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
		}

		return isValid;
	}

#if CE_FRONTEND_USEXLIB
	void *AppFrontend::GetXDisplay() const
	{
		return m_xDisplay;
	}
	int AppFrontend::GetXDefaultScreen() const
	{
		return m_xDefaultScreen;
	}
	#if CE_FRONTEND_USEXCB
		void *AppFrontend::GetXCBConnection() const
		{
			return m_xcbConnection;
		}
	#endif
#endif

#if CE_FRONTEND_USEWIN
	void *AppFrontend::GetHInstance() const
	{
		return m_hInstance;
	}
#endif

	bool AppFrontend::OnEvent(Event &event)
	{
		Canvas *canvas = event.base.canvas;
		return canvas ? canvas->OnEvent(event) : true;
	}
	bool AppFrontend::OnProcess()
	{
		return true;
	}
	bool AppFrontend::OnStart()
	{
		return true;
	}
	bool AppFrontend::OnStop(bool force)
	{
		return true;
	}
}
