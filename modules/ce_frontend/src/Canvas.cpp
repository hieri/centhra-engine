//- Centhra Engine -
#include <CE/Base.h>
#include <CE/AppFrontend.h>
#include <CE/Canvas.h>
#include <CE/Keyboard.h>

#ifdef _WIN32
	//- Windows -
	#include <windows.h>
#endif

//- Standard Library -
#include <cstring>
#include <string>

//- OpenGL -
#include <GL/gl.h>
#include <GL/glu.h>

#ifdef _WIN32
	#include <GL/glext.h>
	#include <GL/wglext.h>
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXTFunc = 0;
#elif __linux__
	#if CE_FRONTEND_USEXLIB
		//- Xlib -
		#include <GL/glext.h>
		#include <X11/Xatom.h>
		#include <X11/Xlib.h>
		#include <GL/glx.h>
		PFNGLXSWAPINTERVALMESAPROC glXSwapIntervalMESAFunc = 0;
		PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGIFunc = 0;

		#if CE_FRONTEND_USEXCB
			//- XCB -
			#include <X11/Xlib-xcb.h>
			#include <xcb/xcb.h>
		#endif
	#endif
#endif


using namespace std;

//TODO: sync mouse button values for cross-platform
//TODO: fix xcb opengl window creation

namespace ce
{
	int g_glxVersionMajor = 0, g_glxVersionMinor = 0;

#if CE_FRONTEND_USEWIN
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
	{
		Canvas *canvas = (Canvas *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if(canvas)
		{
			AppFrontend *app = canvas->GetApp();

			Event event;
			event.base.timeMS = app->GetRunTimeMS();
			event.base.canvas = canvas;
			switch(wMsg)
			{
			case WM_DESTROY:
				PostQuitMessage(0);
				return app->Stop();
			case WM_KEYDOWN:
				event.type = event::KeyDown;
				event.key.scanCode = NativeScanCodeToScanCode((lParam & 0x00FF0000) >> 16);
				event.key.keyCode = ScanCodeToKeyCode(event.key.scanCode);
				event.key.state = 1;
				app->OnEvent(event);
				break;
			case WM_SYSKEYDOWN:
				event.type = event::KeyDown;
				event.key.scanCode = NativeScanCodeToScanCode((lParam & 0x00FF0000) >> 16);
				event.key.keyCode = ScanCodeToKeyCode(event.key.scanCode);
				event.key.state = 1;
				app->OnEvent(event);
				if(event.key.keyCode == Key_F10 || event.key.keyCode == Key_AltLeft || event.key.keyCode == Key_AltRight)
					return 0;
				break;
			case WM_KEYUP:
				event.type = event::KeyUp;
				event.key.scanCode = NativeScanCodeToScanCode((lParam & 0x00FF0000) >> 16);
				event.key.keyCode = ScanCodeToKeyCode(event.key.scanCode);
				event.key.state = 0;
				app->OnEvent(event);
				break;
			case WM_SYSKEYUP:
				event.type = event::KeyUp;
				event.key.scanCode = NativeScanCodeToScanCode((lParam & 0x00FF0000) >> 16);
				event.key.keyCode = ScanCodeToKeyCode(event.key.scanCode);
				event.key.state = 0;
				app->OnEvent(event);
				if(event.key.keyCode == Key_F10 || event.key.keyCode == Key_AltLeft || event.key.keyCode == Key_AltRight)
					return 0;
				break;
			case WM_LBUTTONDOWN:
				event.type = event::MouseButtonDown;
				event.mouseButton.button = event::MouseButtonLeft;
				event.mouseButton.x = LOWORD(lParam);
				event.mouseButton.y = HIWORD(lParam);
				app->OnEvent(event);
				break;
			case WM_LBUTTONUP:
				event.type = event::MouseButtonUp;
				event.mouseButton.button = event::MouseButtonLeft;
				event.mouseButton.x = LOWORD(lParam);
				event.mouseButton.y = HIWORD(lParam);
				app->OnEvent(event);
				break;
			case WM_RBUTTONDOWN:
				event.type = event::MouseButtonDown;
				event.mouseButton.button = event::MouseButtonRight;
				event.mouseButton.x = LOWORD(lParam);
				event.mouseButton.y = HIWORD(lParam);
				app->OnEvent(event);
				break;
			case WM_RBUTTONUP:
				event.type = event::MouseButtonUp;
				event.mouseButton.button = event::MouseButtonRight;
				event.mouseButton.x = LOWORD(lParam);
				event.mouseButton.y = HIWORD(lParam);
				app->OnEvent(event);
				break;
			case WM_MBUTTONDOWN:
				event.type = event::MouseButtonDown;
				event.mouseButton.button = event::MouseButtonMiddle;
				event.mouseButton.x = LOWORD(lParam);
				event.mouseButton.y = HIWORD(lParam);
				app->OnEvent(event);
				break;
			case WM_MBUTTONUP:
				event.type = event::MouseButtonUp;
				event.mouseButton.button = event::MouseButtonMiddle;
				event.mouseButton.x = LOWORD(lParam);
				event.mouseButton.y = HIWORD(lParam);
				app->OnEvent(event);
				break;
			//TODO: Determine if this should be permanently removed
/*			case WM_XBUTTONDOWN:
				event.type = event::MouseButtonDown;
//				event.mouseButton.button = GET_XBUTTON_WPARAM(wParam);
				event.mouseButton.button = event::Unknown;
				event.mouseButton.x = LOWORD(lParam);
				event.mouseButton.y = HIWORD(lParam);
				app->OnEvent(event);
				break;
			case WM_XBUTTONUP: //TODO: Handle this properly
				event.type = event::MouseButtonUp;
//				event.mouseButton.button = GET_XBUTTON_WPARAM(wParam);
				event.mouseButton.button = event::Unknown;
				event.mouseButton.x = LOWORD(lParam);
				event.mouseButton.y = HIWORD(lParam);
				app->OnEvent(event);
				break;
*/			case WM_MOUSEWHEEL:
				event.type = event::MouseWheel;
				event.mouseWheel.delta = GET_WHEEL_DELTA_WPARAM(wParam);
				app->OnEvent(event);
				break;
			case WM_MOUSEMOVE:
				event.type = event::MouseMotion;
				event.mouseMotion.x = LOWORD(lParam);
				event.mouseMotion.y = HIWORD(lParam);
				app->OnEvent(event);
				break;
			case WM_SIZE:
				if(!canvas->IsFullscreen())
					canvas->SetWindowedExtent(LOWORD(lParam), HIWORD(lParam));
				canvas->UpdateViewport(LOWORD(lParam), HIWORD(lParam));
				event.type = event::WindowResize;
				event.windowResize.width = LOWORD(lParam);
				event.windowResize.height = HIWORD(lParam);
				app->OnEvent(event);
				break;
			}
		}

		return DefWindowProc(hWnd, wMsg, wParam, lParam);
	}
#endif

	Canvas *Canvas::Create(int width, int height, const char *title)
	{
		AppFrontend *app = (AppFrontend *)App::GetCurrent();

		if(!app)
		{
			error("[Error] Canvas::Create - There are no Apps running.\n");
			return 0;
		}

		#if CE_FRONTEND_USEXLIB
			Display *xDisplay = (Display *)app->GetXDisplay();
			int xDefaultScreen = app->GetXDefaultScreen();
			GLXContext glxContext;
			GLXWindow glxWindow;

			//TODO: investigate why the Visual Info fails to be received when these int's aren't defined
			int glxVersionMajor = 0, glxVersionMinor = 0;
			if(!glXQueryVersion(xDisplay, &g_glxVersionMajor, &g_glxVersionMinor))
			{
				error("[Error] glxQueryVersion failed.\n");
				return 0;
			}

			#if CE_FRONTEND_USEXCB
				xcb_connection_t *xcbConnection = (xcb_connection_t *)app->GetXCBConnection();
				xcb_screen_t *screen = 0;
				xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(xcbConnection));
				for(int screen_num = xDefaultScreen; screen_iter.rem && screen_num > 0; --screen_num, xcb_screen_next(&screen_iter));
				screen = screen_iter.data;

				xcb_window_t xcbWindow;
				uint32_t xcbEventMask = XCB_EVENT_MASK_KEY_PRESS
					| XCB_EVENT_MASK_KEY_RELEASE
					| XCB_EVENT_MASK_BUTTON_PRESS
					| XCB_EVENT_MASK_BUTTON_RELEASE
					| XCB_EVENT_MASK_POINTER_MOTION
					| XCB_EVENT_MASK_STRUCTURE_NOTIFY
					| XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY;
			#else
				Window xWindow;
				uint32_t xEventMask = KeyPressMask
					| KeyReleaseMask
					| ButtonPressMask
					| ButtonReleaseMask
					| PointerMotionMask
					| StructureNotifyMask
					| SubstructureNotifyMask
					| FocusChangeMask;
			#endif

			if(g_glxVersionMajor >= 1 && g_glxVersionMinor >= 3)
			{
				int fbNumConfig;
				GLint attributeList[] = { GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DOUBLEBUFFER, True, None};
				GLXFBConfig *fbConfig = glXChooseFBConfig(xDisplay, xDefaultScreen, attributeList, &fbNumConfig);

				if(!fbConfig || !fbNumConfig)
				{
					error("[Error] Canvas::Create - glXChooseFBConfig failed.\n");
					return 0;
				}

				glxContext = glXCreateNewContext(xDisplay, fbConfig[0], GLX_RGBA_TYPE, 0, GL_TRUE);
				if(!glxContext)
				{
					error("[Error] Canvas::Create - glXCreateNewContext failed.\n");
					return 0;
				}

				#if CE_FRONTEND_USEXCB
					int visualID = 0;
					glXGetFBConfigAttrib(xDisplay, fbConfig[0], GLX_VISUAL_ID , &visualID);

					xcb_colormap_t xcbColorMap = xcb_generate_id(xcbConnection);
					xcbWindow = xcb_generate_id(xcbConnection);

					xcb_create_colormap(xcbConnection, XCB_COLORMAP_ALLOC_NONE, xcbColorMap, screen->root, visualID);

					uint32_t xcbValueList[] = { xcbEventMask, xcbColorMap, 0 };
					uint32_t xcbValueMask = XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;

					xcb_create_window(xcbConnection, XCB_COPY_FROM_PARENT, xcbWindow, screen->root, 0, 0, width, height, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, visualID, xcbValueMask, xcbValueList);

					if(!xcbWindow)
					{
						glXDestroyContext(xDisplay, glxContext);

						error("[Error] Canvas::Create - xcb_create_window failed.\n");
						return 0;
					}

					xcb_map_window(xcbConnection, xcbWindow);
					xcb_intern_atom_cookie_t cookie = xcb_intern_atom(xcbConnection, 1, 12, "WM_PROTOCOLS");
					xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(xcbConnection, cookie, 0);
					xcb_change_property(xcbConnection, XCB_PROP_MODE_REPLACE, xcbWindow, (*reply).atom, 4, 32, 1, &app->m_wmDeleteMessage);
					glxWindow = glXCreateWindow(xDisplay, fbConfig[0], xcbWindow, 0);

					if(!glxWindow)
					{
						xcb_destroy_window(xcbConnection, xcbWindow);
						glXDestroyContext(xDisplay, glxContext);

						error("[Error] Canvas::Create - glXCreateWindow failed.\n");
						return 0;
					}

					if(!glXMakeContextCurrent(xDisplay, glxWindow, glxWindow, glxContext))
					{
						xcb_destroy_window(xcbConnection, xcbWindow);
						glXDestroyContext(xDisplay, glxContext);

						error("[Error] Canvas::Create - glXMakeContextCurrent failed.\n");
						return 0;
					}
				#else
					XVisualInfo *xVisualInfo = glXGetVisualFromFBConfig(xDisplay, fbConfig[0]);

					if(!xVisualInfo)
					{
						error("[Error] Canvas::Create - glXGetVisualFromFBConfig failed.\n");
						return 0;
					}

					Colormap xColorMap = XCreateColormap(xDisplay, RootWindow(xDisplay, xVisualInfo->screen), xVisualInfo->visual, AllocNone);

					XSetWindowAttributes swa;
					swa.colormap = xColorMap;
					swa.event_mask = xEventMask;
					xWindow = XCreateWindow(xDisplay, RootWindow(xDisplay, xVisualInfo->screen), 0, 0, width, height, 0, xVisualInfo->depth, InputOutput, xVisualInfo->visual, CWColormap | CWEventMask, &swa);
					
					if(!xWindow)
					{
						glXDestroyContext(xDisplay, glxContext);

						error("[Error] Canvas::Create - XCreateWindow failed.\n");
						return 0;
					}

					XMapWindow(xDisplay, xWindow);
					XSetWMProtocols(xDisplay, xWindow, (Atom *)&app->m_wmDeleteMessage, 1);
					glxWindow = glXCreateWindow(xDisplay, fbConfig[0], xWindow, 0);

					if(!glxWindow)
					{
						XDestroyWindow(xDisplay, xWindow);
						glXDestroyContext(xDisplay, glxContext);

						error("[Error] Canvas::Create - glXCreateWindow failed.\n");
						return 0;
					}

					if(!glXMakeContextCurrent(xDisplay, glxWindow, glxWindow, glxContext))
					{
						XDestroyWindow(xDisplay, xWindow);
						glXDestroyContext(xDisplay, glxContext);

						error("[Error] Canvas::Create - glXMakeContextCurrent failed.\n");
						return 0;
					}
				#endif
			}
			else
			{
				#if CE_FRONTEND_USEXCB
					xcb_visualtype_t *xcbVisualInfo = NULL;

					for(xcb_depth_iterator_t depth_iter = xcb_screen_allowed_depths_iterator(screen); depth_iter.rem; xcb_depth_next(&depth_iter))
						for(xcb_visualtype_iterator_t visual_iter = xcb_depth_visuals_iterator(depth_iter.data); visual_iter.rem; xcb_visualtype_next (&visual_iter))
							if(screen->root_visual == visual_iter.data->visual_id)
							{
								xcbVisualInfo = visual_iter.data;
								break;
							}

					if(!xcbVisualInfo)
					{
						//TODO: fix this error message
						error("[Error] Canvas::Create - xcb find visual failed.\n");
						return 0;
					}

					//TODO: investigate if this is bad practice
					XVisualInfo xVisualInfo;
					xVisualInfo.bits_per_rgb = xcbVisualInfo->bits_per_rgb_value;
					xVisualInfo.blue_mask = xcbVisualInfo->blue_mask;
					xVisualInfo.c_class = xcbVisualInfo->_class;
					xVisualInfo.colormap_size = xcbVisualInfo->colormap_entries;
//					xVisualInfo.depth;
					xVisualInfo.green_mask = xcbVisualInfo->green_mask;
					xVisualInfo.red_mask = xcbVisualInfo->red_mask;
//					xVisualInfo.screen;
//					xVisualInfo.visual;
					xVisualInfo.visualid = xcbVisualInfo->visual_id;

					xcb_colormap_t xcbColorMap = xcb_generate_id(xcbConnection);
					xcbWindow = xcb_generate_id(xcbConnection);

					glxContext = glXCreateContext(xDisplay, &xVisualInfo, 0, GL_TRUE);
					if(!glxContext)
					{
						error("[Error] Canvas::Create - glXCreateNewContext failed.\n");
						return 0;
					}

					xcb_create_colormap(xcbConnection, XCB_COLORMAP_ALLOC_NONE, xcbColorMap, screen->root, xcbVisualInfo->visual_id);

					uint32_t xcbValueList[] = { xcbEventMask, xcbColorMap, 0 };
					uint32_t xcbValueMask = XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;

					xcb_create_window(xcbConnection, XCB_COPY_FROM_PARENT, xcbWindow, screen->root, 0, 0, width, height, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, xcbVisualInfo->visual_id, xcbValueMask, xcbValueList);

					if(!xcbWindow)
					{
						glXDestroyContext(xDisplay, glxContext);

						error("[Error] Canvas::Create - xcb_create_window failed.\n");
						return 0;
					}

					xcb_map_window(xcbConnection, xcbWindow);

					if(!glXMakeCurrent(xDisplay, xcbWindow, glxContext))
					{
						xcb_destroy_window(xcbConnection, xcbWindow);
						glXDestroyContext(xDisplay, glxContext);

						error("[Error] Canvas::Create - glXMakeCurrent failed.\n");
						return 0;
					}
				#else
					Window xRootWindow = DefaultRootWindow(xDisplay);

					GLint attributeList[] = { GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DOUBLEBUFFER, True, None};
					XVisualInfo *xVisualInfo = glXChooseVisual(xDisplay, 0, attributeList);

					if(!xVisualInfo)
					{
						error("[Error] Canvas::Create - glXChooseVisual failed.\n");
						return 0;
					}

					glxContext = glXCreateContext(xDisplay, xVisualInfo, 0, GL_TRUE);
					if(!glxContext)
					{
						error("[Error] Canvas::Create - glXCreateNewContext failed.\n");
						return 0;
					}

					Colormap xColorMap = XCreateColormap(xDisplay, xRootWindow, xVisualInfo->visual, AllocNone);

					XSetWindowAttributes swa;
					swa.colormap = xColorMap;
					swa.border_pixel = 0;
					swa.event_mask = xEventMask;

					xWindow = XCreateWindow(xDisplay, xRootWindow, 0, 0, width, height, 0, xVisualInfo->depth, InputOutput, xVisualInfo->visual, CWColormap | CWEventMask, &swa);

					if(!xWindow)
					{
						glXDestroyContext(xDisplay, glxContext);

						error("[Error] Canvas::Create - glXCreateWindow failed.\n");
						return 0;
					}

					XMapWindow(xDisplay, xWindow);

					if(!glXMakeCurrent(xDisplay, xWindow, glxContext))
					{
						XDestroyWindow(xDisplay, xWindow);
						glXDestroyContext(xDisplay, glxContext);

						error("[Error] Canvas::Create - glXMakeCurrent failed.\n");
						return 0;
					}
				#endif
			}
		#endif

		Canvas *canvas = new Canvas();
		canvas->m_app = app;
		canvas->m_width = width;
		canvas->m_height = height;
		canvas->m_windowedWidth = width;
		canvas->m_windowedHeight = height;

		#if CE_FRONTEND_USEWIN
			HINSTANCE hInstance = (HINSTANCE)app->GetHInstance();

			WNDCLASS windowClass;
			windowClass.style = CS_HREDRAW | CS_VREDRAW;
			windowClass.lpfnWndProc = WindowProc;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = hInstance;
			windowClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
			windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			windowClass.lpszMenuName = "";
			windowClass.lpszClassName = "ceApp";

			if(!RegisterClass(&windowClass))
			{
				delete canvas;
				error("[Error] Canvas::Create - RegisterClass failed\n");
				return 0;
			}

			RECT extent;
			UINT style = WS_OVERLAPPEDWINDOW;
			extent.left = 0;
			extent.top = 0;
			extent.right = width;
			extent.bottom = height;
			AdjustWindowRect(&extent, style, FALSE);

			HWND hWnd = CreateWindow("ceApp", title, style, CW_USEDEFAULT, CW_USEDEFAULT, extent.right - extent.left, extent.bottom - extent.top, 0, 0, hInstance, 0);
			if(!hWnd)
			{
				delete canvas;
				error("[Error] Canvas::Create - CreateWindow failed\n");
				return 0;
			}

			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)canvas);

			HDC hDC = GetDC(hWnd);

			PIXELFORMATDESCRIPTOR pfd;
			ZeroMemory(&pfd, sizeof(pfd));
			pfd.nSize = sizeof(pfd);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 32;
			pfd.cDepthBits = 16;
			pfd.iLayerType = PFD_MAIN_PLANE;
			int format = ChoosePixelFormat(hDC, &pfd);
			SetPixelFormat(hDC, format, &pfd);

			HGLRC hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC);

			ShowWindow(hWnd, SW_SHOWNORMAL);
			UpdateWindow(hWnd);
		#endif

		#if CE_FRONTEND_USEXLIB
			canvas->m_glxContext = glxContext;
			canvas->m_glxWindow = glxWindow;

			#if CE_FRONTEND_USEXCB
				xcb_change_property(xcbConnection, XCB_PROP_MODE_REPLACE, xcbWindow, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(title), title);
				canvas->m_xcbWindow = xcbWindow;
				app->m_canvasMap[xcbWindow] = canvas;
			#else
				XStoreName(xDisplay, xWindow, title);
				canvas->m_xWindow = xWindow;
				app->m_canvasMap[xWindow] = canvas;
			#endif
		#endif

		#if CE_FRONTEND_USEWIN
			canvas->m_windowHandle = hWnd;
			canvas->m_deviceContextHandle = hDC;
			canvas->m_glRenderingContextHandle = hRC;
			app->m_canvasMap[hWnd] = canvas;
		#endif
		
		canvas->UpdateViewport(width, height);
		canvas->SetVSync(canvas->m_vsync);

		return canvas;
	}

	Canvas::Canvas()
	{
		m_app = 0;
		m_lastRenderTimeMS = 0;
		m_width = 0;
		m_height = 0;
		m_windowedWidth = 0;
		m_windowedHeight = 0;
		m_vsync = true;
		m_fullscreen = false;

		#if CE_FRONTEND_USEXLIB
			#if CE_FRONTEND_USEXCB
				m_xcbWindow = 0;
				m_glxContext = 0;
				m_glxWindow = 0;
			#else
				m_xWindow = 0;
			#endif
		#endif

		#if CE_FRONTEND_USEWIN
			m_windowClass = "";
			m_windowHandle = 0;
			m_deviceContextHandle = 0;
			m_glRenderingContextHandle = 0;
		#endif
	}
	Canvas::~Canvas()
	{
		#if CE_DISPLAY_USEXLIB
			Display *xDisplay = (Display *)m_app->GetXDisplay();

			glXDestroyContext(xDisplay, (GLXContext *)m_glxContext);
			glXDestroyWindow(xDisplay, m_glxWindow);

			#if CE_DISPLAY_USEXCB
				app->m_canvasMap.erase(m_xcbWindow);
				xcb_connection_t *xcbConnection = (xcb_connection_t *)m_app->GetXCBConnection();
				xcb_destroy_window(xcbConnection, m_xcbWindow);
			#else
				app->m_canvasMap.erase(m_xWindow);
				XDestroyWindow(xDisplay, m_xWindow);
			#endif
		#endif

		#if CE_FRONTEND_USEWIN
			m_app->m_canvasMap.erase(m_windowHandle);

			HINSTANCE hInstance = (HINSTANCE)m_app->GetHInstance();

			wglMakeCurrent((HDC)m_deviceContextHandle, NULL);
			wglDeleteContext((HGLRC)m_glRenderingContextHandle);
			ReleaseDC((HWND)m_windowHandle, (HDC)m_deviceContextHandle);

			UnregisterClass(m_windowClass.c_str(), hInstance);

			CloseWindow((HWND)m_windowHandle);
			DestroyWindow((HWND)m_windowHandle);
		#endif
	}
	AppFrontend *Canvas::GetApp() const
	{
		return m_app;
	}
	void Canvas::Render()
	{
		unsigned long long time = m_app->GetRunTimeMS();

//		print("VS: %d\n", m_vsync);
		//TODO: integrate togglable VSync
//		if((time - m_lastRenderTimeMS) > 15 || !m_vsync)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.f, 0.f, 0.f, 1.f);

			m_lastRenderTimeMS = time;

			Event event;
			event.base.canvas = this;
			event.base.timeMS = time;
			event.type = event::PreRender;
			m_app->OnEvent(event);

			event.base.timeMS = m_app->GetRunTimeMS();
			event.type = event::Render;
			m_app->OnEvent(event);

			event.base.timeMS = m_app->GetRunTimeMS();
			event.type = event::PostRender;
			m_app->OnEvent(event);

			#if CE_FRONTEND_USEXLIB
				Display *xDisplay = (Display *)m_app->GetXDisplay();

				#if CE_FRONTEND_USEXCB
					glXSwapBuffers(xDisplay, (g_glxVersionMajor >= 1 && g_glxVersionMinor >= 3) ? m_glxWindow : m_xcbWindow);
				#else
					glXSwapBuffers(xDisplay, (g_glxVersionMajor >= 1 && g_glxVersionMinor >= 3) ? m_glxWindow : m_xWindow);
				#endif
			#endif
			#if CE_FRONTEND_USEWIN
				SwapBuffers((HDC)m_deviceContextHandle);
			#endif
		}
	}
	bool Canvas::OnEvent(Event &event)
	{
		return true;
	}
	bool Canvas::IsFullscreen() const
	{
		return m_fullscreen;
	}
	void Canvas::SetFullscreen(bool fullscreen)
	{
		if(fullscreen)
		{
			m_fullscreen = fullscreen;
			#if CE_FRONTEND_USEXLIB
				#if CE_FRONTEND_USEXCB
				#else
					Display *xDisplay = (Display *)m_app->GetXDisplay();
					Atom wm_state = XInternAtom(xDisplay, "_NET_WM_STATE", False);
					Atom fullscreen = XInternAtom(xDisplay, "_NET_WM_STATE_FULLSCREEN", False);

					XEvent xev;
					memset(&xev, 0, sizeof(xev));
					xev.type = ClientMessage;
					xev.xclient.window = m_xWindow;
					xev.xclient.message_type = wm_state;
					xev.xclient.format = 32;
					xev.xclient.data.l[0] = 1;
					xev.xclient.data.l[1] = fullscreen;
					xev.xclient.data.l[2] = 0;
					XSendEvent(xDisplay, DefaultRootWindow(xDisplay), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
				#endif
			#endif

			#if CE_FRONTEND_USEWIN
				HWND hwnd = (HWND)m_windowHandle;
				HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
				MONITORINFO mi = { sizeof(mi) };
				if(GetMonitorInfo(hmon, &mi))
				{
					//TODO: Determine if this EXSTYLE is necessary
					SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW);
					SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
					SetWindowPos(hwnd, HWND_NOTOPMOST, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_SHOWWINDOW);
					ShowWindow(hwnd, SW_MAXIMIZE);
				}
			#endif
		}
		else
		{
			#if CE_FRONTEND_USEXLIB
				#if CE_FRONTEND_USEXCB
				#else
					Display *xDisplay = (Display *)m_app->GetXDisplay();
					Atom wm_state = XInternAtom(xDisplay, "_NET_WM_STATE", False);
					Atom fullscreen = XInternAtom(xDisplay, "_NET_WM_STATE_FULLSCREEN", False);

					XEvent xev;
					memset(&xev, 0, sizeof(xev));
					xev.type = ClientMessage;
					xev.xclient.window = m_xWindow;
					xev.xclient.message_type = wm_state;
					xev.xclient.format = 32;
					xev.xclient.data.l[0] = 0;
					xev.xclient.data.l[1] = fullscreen;
					xev.xclient.data.l[2] = 0;
					XSendEvent(xDisplay, DefaultRootWindow(xDisplay), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
				#endif
			#endif

			#if CE_FRONTEND_USEWIN
				HWND hwnd = (HWND)m_windowHandle;
				HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
				MONITORINFO mi = {sizeof(mi)};
				if(GetMonitorInfo(hmon, &mi))
				{
					SetWindowLongPtr(hwnd, GWL_EXSTYLE, 0);
					SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
					ShowWindow(hwnd, SW_RESTORE);

					RECT client, window;
					GetClientRect(hwnd, &client);
					GetWindowRect(hwnd, &window);

					int width = m_windowedWidth + window.right - window.left - client.right + client.left;
					int height = m_windowedHeight + window.bottom - window.top - client.bottom + client.top;
					//TODO: Replace position with default window position instead of center
					SetWindowPos(hwnd, 0, mi.rcMonitor.left + (mi.rcMonitor.right - mi.rcMonitor.left - width) / 2, mi.rcMonitor.top + (mi.rcMonitor.bottom - mi.rcMonitor.top - height) / 2, width, height, SWP_SHOWWINDOW);
				}
			#endif
			m_fullscreen = fullscreen;
		}
	}
	int Canvas::GetWidth() const
	{
		return m_width;
	}
	int Canvas::GetHeight() const
	{
		return m_height;
	}
	void Canvas::UpdateViewport(int width, int height)
	{
		m_width = width;
		m_height = height;

		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.f, (float)width, 0.f, (float)height);
//		glOrtho(0.f, (float)width, 0.f, (float)height, 0.f, 10000.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	void Canvas::SetVSync(bool vsync)
	{
		#ifdef _WIN32
			if(!wglSwapIntervalEXTFunc)
				wglSwapIntervalEXTFunc = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
			wglSwapIntervalEXTFunc(vsync ? 1 : 0);
		#endif

		#ifdef __linux__
			const char *query = glXQueryExtensionsString((Display *)m_app->GetXDisplay(), 0);

			if(strstr(query, "GLX_SGI_swap_control"))
			{
				glXSwapIntervalSGIFunc = (PFNGLXSWAPINTERVALSGIPROC)glXGetProcAddress((GLubyte *)"glXSwapIntervalSGI");
				if(glXSwapIntervalSGIFunc)
					glXSwapIntervalSGIFunc(vsync ? 1 : 0);
			}
			else if(strstr(query, "GLX_MESA_swap_control"))
			{
				glXSwapIntervalMESAFunc = (PFNGLXSWAPINTERVALMESAPROC)glXGetProcAddress((GLubyte *)"glXSwapIntervalMESA");
				if(glXSwapIntervalMESAFunc)
					glXSwapIntervalMESAFunc(vsync ? 1 : 0);
			}
		#endif

		m_vsync = vsync;
	}
	void Canvas::SetWindowedExtent(int width, int height)
	{
		m_windowedWidth = width;
		m_windowedHeight = height;
	}
	void Canvas::SetTitle(const char *title)
	{
		#ifdef _WIN32
			HWND hwnd = (HWND)m_windowHandle;
			SetWindowText(hwnd, title);
		#endif
	}
}
