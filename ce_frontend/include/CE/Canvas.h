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
		static Canvas *create(int width = 64, int height = 64);

		~Canvas();

		void render() const;
	};
}

#endif
