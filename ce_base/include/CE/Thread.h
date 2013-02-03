#ifndef _CE_THREAD_H_
#define _CE_THREAD_H_

//- Standard Library -
#include <stdlib.h>

namespace ce
{
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
		class Attributes
		{
			void *m_pThread_attr;

		public:
			Attributes();
			~Attributes();

			int GetDetachState() const;
			size_t GetStackSize() const;
			void SetDetachState(int detachState);
			void SetStackSize(size_t stackSize);
		};

		static void Exit(void *retVal = 0);

		Thread(void *(*process)(void *));
		~Thread();

		void Start(void *arg = 0, void *attributes = 0);
		void Join(void **ret = 0);
		bool IsRunning() const;

	private:
		Attributes *m_attributes;
	};
}

#endif
