#ifndef _CE_BASE_MUTEX_H_
#define _CE_BASE_MUTEX_H_

//- Centhra Engine -
#include <CE/ConfigBase.h>

namespace ce
{
	class Mutex
	{
		bool m_isAlive;
		#if CE_BASE_USEPOSIXTHREAD
			unsigned long m_pMutex;
		#endif

		#if CE_BASE_USEWINTHREAD
			void *m_mutexHandle;
		#endif

	public:
		Mutex();
		~Mutex();

		void Init();
		void Destroy();
		void Lock();
		void Unlock();
		bool IsAlive() const;
	};
}

#endif
