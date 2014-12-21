//- Centhra Engine -
#include <CE/Counter.h>

namespace ce
{
	Counter::Counter(unsigned long long resolution) : m_resolution(resolution), m_lastPopTime(0), m_count(0)
	{
	}
	void Counter::Increment()
	{
		m_count++;
	}
	bool Counter::PopCount(unsigned long long &time, unsigned long &count)
	{
		if((time - m_lastPopTime) >= m_resolution)
		{
			m_lastPopTime += m_resolution;
			count = m_count;
			m_count = 0;
			return true;
		}
		return false;
	}
}
