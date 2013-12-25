#ifndef _CE_BASE_APP_H_
#define _CE_BASE_APP_H_

//- Standard Library -
#include <map>
#include <string>
#include <vector>

//- Centhra Engine -
#include <CE/ConfigBase.h>

namespace ce
{
	/**	@brief Abstract Application Class
	 */
	class App
	{
		static App *ms_current;

		unsigned long m_startTimeMS;

	protected:
		bool m_isRunning;

	public:
		/**	@brief Returns current application instance.
		 */
		static App *GetCurrent();

		/**	@brief Default Constructor.
		 */
		App();

		/**	@brief Destructor.
		 */
		~App();

		/**	@brief Returns the time since the application was started in milliseconds.
		 *	@return Time since the application was started in milliseconds.
		 */
		unsigned long GetRunTimeMS() const;

		/**	@brief Returns true if the application is running.
		 *	@return Whether or not the application is running.
		 */
		bool IsRunning() const;

		/**	@brief Processes the application run cycle.
		 *	@return Whether or not the application is still running.
		 */
		bool Process();

		/**	@brief Sets the application instance as the current focus.
		 */
		void SetCurrent();

		/**	@brief Attempts to start the application.
		 *	@return Whether or not the application has started.
		 */
		bool Start();

		/**	@brief Attempts to stop the application.
		 *	@param force If true, the stop is considered a force stop.
		 *	@return Whether or not the application has stopped.
		 */
		bool Stop(bool force = false);

		/**	@brief Called when the application outputs an error message.
		 */
		virtual void OnError(const char *str);

		/**	@brief Called when the application outputs a message.
		 */
		virtual void OnPrint(const char *str);

		/**	@brief Called when the application run cycle is processed.
		 *	@return Whether or not the application should keep running.
		 */
		virtual bool OnProcess();

		/**	@brief Called when the application is asked to start.
		 *	@return Whether or not the application should start.
		 */
		virtual bool OnStart();

		/**	@brief Called when the application has started.
		 */
		virtual void OnStarted();

		/**	@brief Called when the application is asked to stop.
		 *	@param force If true, the stop is considered a force stop.
		 *	@return Whether or not the application should stop.
		 */
		virtual bool OnStop(bool force = false);

		/**	@brief Called when the application has stopped.
		 */
		virtual void OnStopped();

		/**	@brief Called when the application outputs a warning message.
		 */
		virtual void OnWarn(const char *str);
	};
}

#endif
