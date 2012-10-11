#include <assert.h>

//- POSIX -
#include <pthread.h>

//- Centhra Engine -
#include <CE/Thread.h>

namespace ce
{
	Thread::Thread(void *(*process)(void *))
	{
		m_attributes = 0;
		m_process = process;
		m_pthread = 0;
	}
	Thread::~Thread()
	{
		if(m_pthread)
			join(NULL);
	}
	void Thread::start(void *arg, void *attributes)
	{
		assert(m_pthread == 0);
		pthread_create((pthread_t *)&m_pthread, (const pthread_attr_t *)attributes, m_process, arg);
	}
	void Thread::exit(void *retVal)
	{
		pthread_exit(retVal);
	}
	void Thread::join(void **ret)
	{
		assert(m_pthread != 0);
		pthread_join(m_pthread, ret);
		m_pthread = 0;
	}
	bool Thread::isRunning() const
	{
		return m_pthread != 0;
	}

	Thread::Attributes::Attributes()
	{
		pthread_attr_t *attr = new pthread_attr_t;
		pthread_attr_init(attr);
		m_pthread_attr = attr;
	}
	Thread::Attributes::~Attributes()
	{
		pthread_attr_destroy((pthread_attr_t *)m_pthread_attr);
	}
	int Thread::Attributes::getDetachState() const
	{
		int detachState;
		pthread_attr_getdetachstate((pthread_attr_t *)m_pthread_attr, &detachState);
		return detachState;
	}
	size_t Thread::Attributes::getStackSize() const
	{
		size_t stackSize;
		pthread_attr_getstacksize((pthread_attr_t *)m_pthread_attr, &stackSize);
		return stackSize;
	}
	void Thread::Attributes::setDetachState(int detachState)
	{
		pthread_attr_setdetachstate((pthread_attr_t *)m_pthread_attr, detachState);
	}
	void Thread::Attributes::setStackSize(size_t stackSize)
	{
		pthread_attr_setstacksize((pthread_attr_t *)m_pthread_attr, stackSize);
	}
}
