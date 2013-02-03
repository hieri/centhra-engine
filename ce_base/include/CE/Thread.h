#ifndef _CE_THREAD_H_
#define _CE_THREAD_H_

//- Standard Library -
#include <stdlib.h>

namespace ce
{
	/**	@brief Thread Data Structure Class
	 */
	class Thread
	{
		#if CE_BASE_USEPOSIXTHREAD
			unsigned long m_pThread;
		#endif
		#if CE_BASE_USEWINTHREAD
			void *m_threadHandle;
		#endif

		void *(*m_process)(void *);

	public:
		/**	@brief Exits the thread function.
		 *	@param retVal Return value of the thread.
		 */
		static void Exit(void *retVal = 0);

		/**	@brief Constructor.
		 *	@param process Thread function.
		 */
		Thread(void *(*process)(void *));

		/**	@brief Destructor.
		 */
		~Thread();

		/**	@brief Starts the thread.
		 *	@param arg Argument passed to the thread function.
		 *	@param attributes Attributes for the thread.
		 */
		void Start(void *arg = 0, void *attributes = 0);

		/**	@brief Joins the thread with the current thread.
		 *	@param ret Stores the return value of the thread.
		 */
		void Join(void **ret = 0);

		/**	@brief Returns true if the thread is running.
		 *	@return Whether or not the thraed is running.
		 */
		bool IsRunning() const;
	};
}

#endif
