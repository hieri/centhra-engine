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
		//- TODO: Find more efficient way of attaching user pointers to OS specific objects. -
		//-   Xlib: XAssocTable -
		//-   Windows: [Set,Get]WindowLongPtr(hWnd, GWLP_USERDATA[, canvasPtr]); -

		#if CE_FRONTEND_USEXLIB
			void *m_xDisplay;
			int m_xDefaultScreen;
			unsigned long m_wmDeleteMessage;
			#if CE_FRONTEND_USEXCB
				void *m_xcbConnection;
			#endif
			std::map<int, Canvas *> m_canvasMap;
		#endif

		#if CE_FRONTEND_USEWIN
			void *m_hInstance;
			std::map<void *, Canvas *> m_canvasMap;
		#endif

	public:
		AppFrontend();
		~AppFrontend();

		bool Process();
		bool Start();
		bool Stop(bool force = false);

		#if CE_FRONTEND_USEXLIB
			void *GetXDisplay() const;
			int GetXDefaultScreen() const;
			#if CE_FRONTEND_USEXCB
				void *GetXCBConnection() const;
			#endif
		#endif

		#if CE_FRONTEND_USEWIN
			void *GetHInstance() const;
		#endif

		//- User-Defined Functions -
		virtual bool OnEvent(Event &event);
		virtual bool OnProcess();
		virtual bool OnStart();
		virtual bool OnStop(bool force = false);

		friend class Canvas;
	};
}

#endif
