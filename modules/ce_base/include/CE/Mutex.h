#ifndef _CE_BASE_MUTEX_H_
#define _CE_BASE_MUTEX_H_

//- Centhra Engine -
#include <CE/ConfigBase.h>

namespace ce
{
	class Mutex
	{
		#if CE_BASE_USEPOSIXTHREAD
			unsigned long m_pMutex;
		#endif

	public:
		Mutex();
		~Mutex();

		void Init();
		void Destroy();
		void Lock();
		void Unlock();
	};
}

#endif
