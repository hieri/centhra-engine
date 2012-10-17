//- Centhra Engine -
#include <CE/Base.h>
#include <CE/AppFrontend.h>
#include <CE/Canvas.h>

#ifdef _WIN32
	#include <windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

#if CE_FRONTEND_USEXLIB
	//- Xlib -
	#include <X11/Xlib.h>
	#include <GL/glx.h>


	#if CE_FRONTEND_USEXCB
		//- XCB -
		#include <X11/Xlib-xcb.h>
		#include <xcb/xcb.h>
	#endif
#endif

namespace ce
{
	Canvas *Canvas::create()
	{
		AppFrontend *app = (AppFrontend *)App::getCurrent();

		if(!app)
		{
			error("[Error] Canvas::create - There are no Apps running.\n");
		}

		#if CE_FRONTEND_USEXLIB
			Display *xDisplay = (Display *)app->getXDisplay();
			int xDefaultScreen = app->getXDefaultScreen();

			#if CE_FRONTEND_USEXCB
				xcb_connection_t *xcbConnection = (xcb_connection_t *)app->getXCBConnection();
				xcb_screen_t *screen = 0;
				xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(xcbConnection));
				for(int screen_num = xDefaultScreen; screen_iter.rem && screen_num > 0; --screen_num, xcb_screen_next(&screen_iter));
				screen = screen_iter.data;

				int visualID = 0;

				// Query framebuffer configurations
				GLXFBConfig *fb_configs = 0;
				int num_fb_configs = 0;
				fb_configs = glXGetFBConfigs(xDisplay, xDefaultScreen, &num_fb_configs);
				if(!fb_configs || num_fb_configs == 0)
				{
					error("[Error] Window::create - glXGetFBConfigs failed\n");
					return 0;
				}

				// Select first framebuffer config and query visualID
				GLXFBConfig fb_config = fb_configs[0];
				glXGetFBConfigAttrib(xDisplay, fb_config, GLX_VISUAL_ID , &visualID);

				GLXContext glxContext;

				// Create OpenGL context
				glxContext = glXCreateNewContext(xDisplay, fb_config, GLX_RGBA_TYPE, 0, True);
				if(!glxContext)
				{
					error("[Error] Window::create - glXCreateNewContext failed\n");
					return 0;
				}

				xcb_colormap_t colormap = xcb_generate_id(xcbConnection);
				xcb_window_t xcbWindow = xcb_generate_id(xcbConnection);

				xcb_create_colormap(xcbConnection, XCB_COLORMAP_ALLOC_NONE, colormap, screen->root, visualID);
				
				uint32_t eventmask = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE;
				uint32_t valuelist[] = { eventmask, colormap, 0 };
				uint32_t valuemask = XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;

				xcb_create_window(xcbConnection, XCB_COPY_FROM_PARENT, xcbWindow, screen->root, 0, 0, 150, 150, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, visualID, valuemask, valuelist);
				xcb_map_window(xcbConnection, xcbWindow);

				// Create GLX Window
				GLXDrawable drawable = 0;

				GLXWindow glxWindow = 
					glXCreateWindow(
						xDisplay,
						fb_config,
						xcbWindow,
						0
						);

				if(!xcbWindow)
				{
					xcb_destroy_window(xcbConnection, xcbWindow);
					glXDestroyContext(xDisplay, glxContext);

					error("[Error] Window::create - glXDestroyContext failed\n");
					return 0;
				}

				drawable = glxWindow;

				// make OpenGL context current
				if(!glXMakeContextCurrent(xDisplay, drawable, drawable, glxContext))
				{
					xcb_destroy_window(xcbConnection, xcbWindow);
					glXDestroyContext(xDisplay, glxContext);

					error("[Error] Window::create - glXMakeContextCurrent failed\n");
					return 0;
				}
			#else
				Window xWindow = XCreateSimpleWindow(xDisplay, RootWindow(xDisplay, xDefaultScreen), 10, 10, 100, 100, 1, BlackPixel(xDisplay, xDefaultScreen), WhitePixel(xDisplay, xDefaultScreen));
				XSelectInput(xDisplay, xWindow, ExposureMask | KeyPressMask | KeyReleaseMask);
				XMapWindow(xDisplay, xWindow);
			#endif
		#endif

		#if CE_FRONTEND_USEWIN
			HINSTANCE hInstance = (HINSTANCE)app->getHInstance();

			WNDCLASS windowClass;
			windowClass.style = CS_HREDRAW | CS_VREDRAW;
			windowClass.lpfnWndProc = DefWindowProc;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = hInstance;
			windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
			windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			windowClass.lpszMenuName = "";
			windowClass.lpszClassName = "ceApp";

			if(!RegisterClass(&windowClass))
			{
				error("[Error] Window::create - RegisterClass failed\n");
				return 0;
			}

			HWND hWnd = CreateWindow("ceApp", "fasdf", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 100, 100, 0, 0, hInstance, 0);
			if(!hWnd)
			{
				error("[Error] Window::create - CreateWindow failed\n");
				return 0;
			}

			ShowWindow(hWnd, SW_SHOWNORMAL);
			UpdateWindow(hWnd);
		#endif

		Canvas *canvas = new Canvas();

		#if CE_FRONTEND_USEXLIB
			#if CE_FRONTEND_USEXCB
				canvas->m_xcbWindow = xcbWindow;
				canvas->m_glxContext = glxContext;
				canvas->m_glxWindow = glxWindow;
			#else
				canvas->m_xWindow = xWindow;
			#endif
		#endif

		#if CE_FRONTEND_USEWIN
			canvas->m_windowHandle = hWnd;
			app->m_canvasMap[hWnd] = canvas;
		#endif
		
		return canvas;
	}

	Canvas::Canvas()
	{
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
		#endif
	}
	Canvas::~Canvas()
	{
		AppFrontend *app = (AppFrontend *)App::getCurrent();

		#if CE_DISPLAY_USEXLIB
			Display *xDisplay = (Display *)app->getXDisplay();

			#if CE_DISPLAY_USEXCB
				xcb_connection_t *xcbConnection = (xcb_connection_t *)app->getXCBConnection();
				glXDestroyWindow(xDisplay, m_glxWindow);
				xcb_destroy_window(xcbConnection, m_xcbWindow);
				glXDestroyContext(xDisplay, (GLXContext *)m_glxContext);
			#else
				XDestroyWindow(xDisplay, m_xWindow);
			#endif
		#endif

		#if CE_FRONTEND_USEWIN
			HINSTANCE hInstance = (HINSTANCE)app->getHInstance();
			UnregisterClass(m_windowClass.c_str(), hInstance);
			CloseWindow((HWND)m_windowHandle);
			DestroyWindow((HWND)m_windowHandle);
			app->m_canvasMap.erase(this);
		#endif
	}
}
