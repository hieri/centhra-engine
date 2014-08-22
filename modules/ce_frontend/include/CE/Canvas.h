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

	/**	@brief Canvas Class
	 */
	class Canvas
	{
		AppFrontend *m_app;
		bool m_vsync, m_fullscreen;
		unsigned long long m_lastRenderTimeMS;
		int m_width, m_height, m_windowedWidth, m_windowedHeight;

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
		/**	@brief Constructor.
		 *	@param width Width of the canvas.
		 *	@param height Height of the canvas.
		 *	@param title Title of the canvas.
		 *	@return Canvas object or 0 if creation failed.
		 */
		static Canvas *Create(int width = 64, int height = 64, const char *title = "");

		/**	@brief Destructor.
		 */
		~Canvas();

		/**	@brief Returns the application that owns the canvas.
		 *	@return Application that owns the canvas.
		 */
		AppFrontend *GetApp() const;

		/**	@brief Renders the canvas.
		 */
		void Render();

		/**	@brief Called when an event is queued by the canvas.
		 *	@param event Queued Event.
		 *	@return Whether or not the event should be processed.
		 */
		virtual bool OnEvent(Event &event);

		bool IsFullscreen() const;
		void SetFullscreen(bool fullscreen);

		int GetWidth() const;
		int GetHeight() const;
		void UpdateViewport(int width, int height);
		void SetVSync(bool vsync);
		void SetWindowedExtent(int width, int height);

		void SetTitle(const char *title);
	};
}

#endif
