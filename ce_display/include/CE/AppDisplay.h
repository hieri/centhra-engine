#ifndef _CE_APP_DISPLAY_H_
#define _CE_APP_DISPLAY_H_

//- Centhra Engine -
#include <CE/App.h>
#include <CE/ConfigDisplay.h>

namespace ce
{
	class AppDisplay : public App
	{
		#if CE_DISPLAY_USEXLIB
			void *m_xDisplay;
			int m_xDefaultScreen;
			#if CE_DISPLAY_USEXCB
				void *m_xcbConnection;
			#endif
		#endif

	public:
		AppDisplay();
		~AppDisplay();

		bool process();
		bool quit();
		bool start();

		#if CE_DISPLAY_USEXLIB
			void *getXDisplay() const;
			int getXDefaultScreen() const;
			#if CE_DISPLAY_USEXCB
				void *getXCBConnection() const;
			#endif
		#endif

		//- User-Defined Functions -
		virtual bool onLoop();
		virtual bool onQuit();
		virtual bool onStart();
	};
}

#endif
