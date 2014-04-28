//- Centhra Engine -
#include <CE/Mutex.h>

#if CE_BASE_USEPOSIXTHREAD
	//- POSIX -
	#include <pthread.h>
#endif

#if CE_BASE_USEWINTHREAD
	//- WinAPI -
	#include <Windows.h>
#endif

namespace ce
{
	Mutex::Mutex()
	{
		#if CE_BASE_USEPOSIXTHREAD
			m_pMutex = 0;
		#endif

		#if CE_BASE_USEWINTHREAD
			m_mutexHandle = 0;
		#endif
		m_isAlive = false;
	}
	Mutex::~Mutex()
	{
		Destroy();
	}
	void Mutex::Lock()
	{
		#if CE_BASE_USEPOSIXTHREAD
			pthread_mutex_lock((pthread_mutex_t *)m_pMutex);
		#endif

		#if CE_BASE_USEWINTHREAD
			WaitForSingleObject(m_mutexHandle, INFINITE);
		#endif
	}
	void Mutex::Unlock()
	{
		#if CE_BASE_USEPOSIXTHREAD
			pthread_mutex_unlock((pthread_mutex_t *)m_pMutex);
		#endif

		#if CE_BASE_USEWINTHREAD
			ReleaseMutex(m_mutexHandle);
		#endif
	}
	void Mutex::Init()
	{
		if(m_isAlive)
			return;

		#if CE_BASE_USEPOSIXTHREAD
			m_pMutex = new pthread_mutex_t;
			pthread_mutex_init((pthread_mutex_t *)m_pMutex, 0);
		#endif

		#if CE_BASE_USEWINTHREAD
			m_mutexHandle = CreateMutex(NULL, FALSE, NULL);
		#endif

		m_isAlive = true;
	}
	void Mutex::Destroy()
	{
		if(!m_isAlive)
			return;

		#if CE_BASE_USEPOSIXTHREAD
			pthread_mutex_destroy((pthread_mutex_t *)m_pMutex);
			delete (pthread_mutex_t *)m_pMutex;
			m_pMutex = 0;
		#endif

		#if CE_BASE_USEWINTHREAD
			CloseHandle(m_mutexHandle);
		#endif

		m_isAlive = false;
	}
	bool Mutex::IsAlive() const
	{
		return m_isAlive;
	}
}
