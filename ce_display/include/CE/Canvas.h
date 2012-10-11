#ifndef _CE_CANVAS_H_
#define _CE_CANVAS_H_

//- Centhra Engine -
#include <CE/ConfigDisplay.h>

namespace ce
{
	class Canvas
	{
		#if CE_DISPLAY_USEXLIB
			#if CE_DISPLAY_USEXCB
				int m_xcbWindow;
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
