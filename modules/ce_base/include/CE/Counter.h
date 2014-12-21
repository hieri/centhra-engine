#ifndef _CE_COUNTER_H_
#define _CE_COUNTER_H_

namespace ce
{
	class Counter
	{
	protected:
		unsigned long long m_lastPopTime, m_resolution;
		unsigned long m_count;
	public:
		Counter(unsigned long long resolution);

		void Increment();
		bool PopCount(unsigned long long &time, unsigned long &count);
	};
}

#endif
