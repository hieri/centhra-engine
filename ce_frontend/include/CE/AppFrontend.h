#ifndef _CE_APP_FRONTEND_H_
#define _CE_APP_FRONTEND_H_

//- Centhra Engine -
#include <CE/App.h>
#include <CE/ConfigFrontend.h>
#include <CE/Event.h>

//- Standard Library -
#include <map>

namespace ce
{
	class Canvas;

	class AppFrontend : public App
	{
		#if CE_FRONTEND_USEXLIB
			void *m_xDisplay;
			int m_xDefaultScreen;
			#if CE_FRONTEND_USEXCB
				void *m_xcbConnection;
			#endif
		#endif

		#if CE_FRONTEND_USEWIN
			void *m_hInstance;
			std::map<void *, Canvas *> m_canvasMap;
		#endif

	public:
		AppFrontend();
		~AppFrontend();

		bool process();
		bool quit();
		bool start();

		#if CE_FRONTEND_USEXLIB
			void *getXDisplay() const;
			int getXDefaultScreen() const;
			#if CE_FRONTEND_USEXCB
				void *getXCBConnection() const;
			#endif
		#endif

		#if CE_FRONTEND_USEWIN
			void *getHInstance() const;
		#endif

		//- User-Defined Functions -
		virtual bool onEvent(Event &event);
		virtual bool onLoop();
		virtual bool onQuit();
		virtual bool onStart();

		friend class Canvas;
	};
}

#endif