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

	/**	@brief Frontend Application Class
	 */
	class AppFrontend : public App
	{
		//TODO: Find more efficient way of attaching user pointers to OS specific objects.
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
		/**	@brief Default Constructor.
		 */
		AppFrontend();

		/**	@brief Destructor.
		 */
		~AppFrontend();

		/**	@brief Processes the application run cycle.
		 *	@return Whether or not the application is still running.
		 */
		bool Process();

		/**	@brief Attempts to start the application.
		 *	@return Whether or not the application has started.
		 */
		bool Start();

		/**	@brief Attempts to stop the application.
		 *	@param force If true, the stop is considered a force stop.
		 *	@return Whether or not the application has stopped.
		 */
		bool Stop(bool force = false);

		#if CE_FRONTEND_USEXLIB
			/**	@brief [x11 Only] Returns the XDisplay of the application.
			 *	@return XDisplay of the application.
			 */
			void *GetXDisplay() const;

			/**	@brief [x11 Only] Returns the XDefaultScreen of the application.
			 *	@return XDefaultScreen of the application.
			 */
			int GetXDefaultScreen() const;

			#if CE_FRONTEND_USEXCB
				/**	@brief [x11 + xcb Only] Returns the XCBConnection of the application.
				 *	@return XCBConnection of the application.
				 */
				void *GetXCBConnection() const;
			#endif
		#endif

		#if CE_FRONTEND_USEWIN
			/**	@brief [Windows Only] Returns the HInstance of the application.
			 *	@return HInstance of the application.
			 */
			void *GetHInstance() const;
		#endif

		/**	@brief Called when an event is queued by the application.
		 *	@param event Queued Event.
		 *	@return Whether or not the event should be processed.
		 */
		virtual bool OnEvent(Event &event);

		/**	@brief Called when the application run cycle is processed.
		 *	@return Whether or not the application should keep running.
		 */
		virtual bool OnProcess();

		/**	@brief Called when the application is asked to start.
		 *	@return Whether or not the application should start.
		 */
		virtual bool OnStart();

		/**	@brief Called when the application is asked to stop.
		 *	@param force If true, the stop is considered a force stop.
		 *	@return Whether or not the application should stop.
		 */
		virtual bool OnStop(bool force = false);

		friend class Canvas;
	};
}

#endif
