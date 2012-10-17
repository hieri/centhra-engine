#ifndef _CE_CANVAS_H_
#define _CE_CANVAS_H_

//- Centhra Engine -
#include <CE/ConfigFrontend.h>

//- Standard Library -
#include <string>

namespace ce
{
	class Canvas
	{
		#if CE_FRONTEND_USEXLIB
			#if CE_FRONTEND_USEXCB
				int m_xcbWindow, m_glxWindow;
				void *m_glxContext;
			#else
				int m_xWindow;
			#endif
		#endif

		#if CE_FRONTEND_USEWIN
			std::string m_windowClass;
			void *m_windowHandle;
		#endif

	protected:
		Canvas();

	public:
		static Canvas *create();

		~Canvas();
	};
}

#endif
