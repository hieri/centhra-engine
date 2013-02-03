#ifndef _CE_APP_H_
#define _CE_APP_H_

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

		bool m_isRunning;
		unsigned long m_startTimeMS;

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
		 */
		unsigned long GetRunTimeMS() const;

		/**	@brief Returns true if the application is running.
		 */
		bool IsRunning() const;

		/**	@brief Processes the application run cycle and returns true if the application is still running.
		 */
		bool Process();

		/**	@brief Sets the application instance as the current focus.
		 */
		void SetCurrent();

		/**	@brief Attempts to start the application and returns true if successful.
		 */
		bool Start();

		/**	@brief Attempts to stop the application and returns true if successful.
		 */
		bool Stop(bool force = false);

		/**	@brief Called when the application run cycle is processed.
		 *	The return value determines whether or not the application should keep running.
		 */
		virtual bool OnProcess();

		/**	@brief Called when the application is asked to start.
		 *	The return value determines whether or not the application has successfully started.
		 */
		virtual bool OnStart();

		/**	@brief Called when the application is asked to stop.
		 *	The return value determines whether or not the application has successfulled stopped.
		 */
		virtual bool OnStop(bool force = false);
	};
}

#endif
