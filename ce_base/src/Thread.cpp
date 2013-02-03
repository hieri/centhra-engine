#include <assert.h>

//- Centhra Engine -
#include <CE/ConfigBase.h>
#include <CE/Thread.h>

#if CE_BASE_USEPOSIXTHREAD
	//- POSIX -
	#include <pthread.h>
#endif

#ifdef _WIN32
	#include <windows.h>
#endif

namespace ce
{
	Thread::Thread(void *(*process)(void *))
	{
		m_attributes = 0;
		m_process = process;

		#if CE_BASE_USEPOSIXTHREAD
			m_pThread = 0;
		#endif

		#if CE_BASE_USEWINTHREAD
			m_threadHandle = 0;
		#endif
	}
	Thread::~Thread()
	{
		#if CE_BASE_USEPOSIXTHREAD
			if(m_pThread)
		#endif

		#if CE_BASE_USEWINTHREAD
			if(m_threadHandle)
		#endif
				Join(NULL);
	}
	void Thread::Start(void *arg, void *attributes)
	{
		#if CE_BASE_USEPOSIXTHREAD
			assert(m_pThread == 0);
			pthread_create((pthread_t *)&m_pThread, (const pthread_attr_t *)attributes, m_process, arg);
		#endif

		#if CE_BASE_USEWINTHREAD
			assert(m_threadHandle == 0);
			m_threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)m_process, arg, 0, 0);
		#endif
	}
	void Thread::Exit(void *retVal)
	{
		#if CE_BASE_USEPOSIXTHREAD
			pthread_exit(retVal);
		#endif
	}
	void Thread::Join(void **ret)
	{
		#if CE_BASE_USEPOSIXTHREAD
			assert(m_pThread != 0);
			pthread_join(m_pThread, ret);
			m_pThread = 0;
		#endif

		#if CE_BASE_USEWINTHREAD
			assert(m_threadHandle != 0);
			WaitForSingleObject((HANDLE)m_threadHandle, INFINITE);
			CloseHandle((HANDLE)m_threadHandle);
			m_threadHandle = 0;
		#endif
	}
	bool Thread::IsRunning() const
	{
		#if CE_BASE_USEPOSIXTHREAD
			return m_pThread != 0;
		#endif

		#if CE_BASE_USEWINTHREAD
			return m_threadHandle != 0;
		#endif

		return false;
	}

	Thread::Attributes::Attributes()
	{
		#if CE_BASE_USEPOSIXTHREAD
			pthread_attr_t *attr = new pthread_attr_t;
			pthread_attr_init(attr);
			m_pThread_attr = attr;
		#endif
	}
	Thread::Attributes::~Attributes()
	{
		#if CE_BASE_USEPOSIXTHREAD
			pthread_attr_destroy((pthread_attr_t *)m_pThread_attr);
		#endif
	}
	int Thread::Attributes::GetDetachState() const
	{
		int detachState = 0;
		#if CE_BASE_USEPOSIXTHREAD
			pthread_attr_getdetachstate((pthread_attr_t *)m_pThread_attr, &detachState);
		#endif
		return detachState;
	}
	size_t Thread::Attributes::GetStackSize() const
	{
		size_t stackSize = 0;
		#if CE_BASE_USEPOSIXTHREAD
			pthread_attr_getstacksize((pthread_attr_t *)m_pThread_attr, &stackSize);
		#endif
		return stackSize;
	}
	void Thread::Attributes::SetDetachState(int detachState)
	{
		#if CE_BASE_USEPOSIXTHREAD
			pthread_attr_setdetachstate((pthread_attr_t *)m_pThread_attr, detachState);
		#endif
	}
	void Thread::Attributes::SetStackSize(size_t stackSize)
	{
		#if CE_BASE_USEPOSIXTHREAD
			pthread_attr_setstacksize((pthread_attr_t *)m_pThread_attr, stackSize);
		#endif
	}
}
