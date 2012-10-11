#ifndef _CE_CANVAS_H_
#define _CE_CANVAS_H_

//- Centhra Engine -
#include <CE/ConfigFrontend.h>

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

	protected:
		Canvas();

	public:
		static Canvas *create();

		~Canvas();
	};
}

#endif
