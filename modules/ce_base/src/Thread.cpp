//- Standard Library -
#include <assert.h>
#include <vector>

#ifdef _WIN32
	#include <windows.h>
#endif

//- Centhra Engine -
#include <CE/ConfigBase.h>
#include <CE/Base.h>
#include <CE/Thread.h>

#if CE_BASE_USEPOSIXTHREAD
	//- POSIX -
	#include <pthread.h>
#endif

using namespace std;

namespace ce
{
	vector<Thread *> g_deadThreads;

	void Thread::FinalizeDelete()
	{
		if(g_deadThreads.empty() == false)
		{
			Thread **markThreads = &g_deadThreads.front();
			Thread **endThreads = markThreads + g_deadThreads.size();
			while(markThreads != endThreads)
				delete *markThreads++;
			g_deadThreads.clear();
		}
	}
	Thread::Thread(void *(*process)(void *))
	{
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

		#if CE_BASE_USEWINTHREAD
			ExitThread((DWORD)retVal);
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
	void Thread::End()
	{
		g_deadThreads.push_back(this);
	}
}
