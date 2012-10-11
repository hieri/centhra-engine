#ifndef _CE_THREAD_H_
#define _CE_THREAD_H_

//- Standard Library -
#include <stdio.h>

namespace ce
{
	class Thread
	{
		unsigned long m_pthread;
		void *(*m_process)(void *);

	public:
		class Attributes
		{
			void *m_pthread_attr;

		public:
			Attributes();
			~Attributes();

			int getDetachState() const;
			size_t getStackSize() const;
			void setDetachState(int detachState);
			void setStackSize(size_t stackSize);
		};

		static void exit(void *retVal = 0);

		Thread(void *(*process)(void *));
		~Thread();

		void start(void *arg = 0, void *attributes = 0);
		void join(void **ret = 0);
		bool isRunning() const;

	private:
		Attributes *m_attributes;
	};
}

#endif
