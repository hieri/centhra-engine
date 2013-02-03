#ifndef _CE_CANVAS_H_
#define _CE_CANVAS_H_

//- Centhra Engine -
#include <CE/ConfigFrontend.h>
#include <CE/Event.h>

//- Standard Library -
#include <string>

namespace ce
{
	class AppFrontend;

	class Canvas
	{
		AppFrontend *m_app;
		unsigned long m_lastRenderTimeMS;

		#if CE_FRONTEND_USEXLIB
			int m_glxWindow;
			void *m_glxContext;
			#if CE_FRONTEND_USEXCB
				int m_xcbWindow;
			#else
				int m_xWindow;
			#endif
		#endif

		#if CE_FRONTEND_USEWIN
			std::string m_windowClass;
			void *m_windowHandle, *m_deviceContextHandle, *m_glRenderingContextHandle;
		#endif

	protected:
		Canvas();

	public:
		static Canvas *Create(int width = 64, int height = 64, const char *title = "");

		~Canvas();

		AppFrontend *GetApp() const;
		void Render();

		//- User-Defined Functions -
		virtual bool OnEvent(Event &event);
	};
}

#endif
