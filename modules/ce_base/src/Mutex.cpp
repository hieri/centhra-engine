//- Centhra Engine -
#include <CE/Mutex.h>

#if CE_BASE_USEPOSIXTHREAD
	//- POSIX -
	#include <pthread.h>
#endif

namespace ce
{
	Mutex::Mutex()
	{
		m_pMutex = 0;
		m_isAlive = false;
	}
	Mutex::~Mutex()
	{
		Destroy();
	}
	void Mutex::Lock()
	{
		#if CE_BASE_USEPOSIXTHREAD
			pthread_mutex_lock((pthread_mutex_t *)&m_pMutex);
		#endif
	}
	void Mutex::Unlock()
	{
		#if CE_BASE_USEPOSIXTHREAD
			pthread_mutex_unlock((pthread_mutex_t *)&m_pMutex);
		#endif
	}
	void Mutex::Init()
	{
		if(m_isAlive)
			return;

		#if CE_BASE_USEPOSIXTHREAD
			pthread_mutex_init((pthread_mutex_t *)&m_pMutex, 0);
		#endif

		//- Windows: OpenMutex -

		m_isAlive = true;
	}
	void Mutex::Destroy()
	{
		if(!m_isAlive)
			return;

		#if CE_BASE_USEPOSIXTHREAD
			pthread_mutex_destroy((pthread_mutex_t *)&m_pMutex);
		#endif

		//- Windows: ReleaseMutex -

		m_isAlive = false;
	}
	bool Mutex::IsAlive() const
	{
		return m_isAlive;
	}
}
