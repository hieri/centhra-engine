#ifndef _CE_CANVAS_H_
#define _CE_CANVAS_H_

//- Standard Library -
#include <string>

//- Centhra Engine -
#include <CE/ConfigBase.h>
#include <CE/Event.h>
#include <CE/Matrix4x4.h>

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
		int_canvas m_width, m_height, m_windowedWidth, m_windowedHeight;

		#if CE_BASE_USEXLIB
			int m_glxWindow;
			void *m_glxContext;
			#if CE_BASE_USEXCB
				int m_xcbWindow;
			#else
				int m_xWindow;
			#endif
		#endif

		#if CE_BASE_USEWIN
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
		static Canvas *Create(int_canvas width = 64, int_canvas height = 64, const char *title = "");

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

		int_canvas GetWidth() const;
		int_canvas GetHeight() const;
		void UpdateViewport(int_canvas width, int_canvas height);
		void SetVSync(bool vsync);
		void SetWindowedExtent(int_canvas width, int_canvas height);
		void Resize(int_canvas width, int_canvas height);

		void SetTitle(const char *title);

		//- DPI -
	protected:
		float m_horizontalDpi, m_verticalDpi;
	public:
		float GetHorizontalDPI() const;
		float GetVerticalDPI() const;

		//- Projection Matrix -
	protected:
		Matrix4x4<float> m_projectionMatrix;
		void UpdateProjectionMatrix();
	public:
		inline Matrix4x4<float> GetProjectionMatrix() const
		{
			return m_projectionMatrix;
		}
	};
}

#endif
