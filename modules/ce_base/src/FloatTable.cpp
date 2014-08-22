//- Centhra Engine -
#include <CE/FloatTable.h>

using namespace ce;

namespace ce
{
	FloatTable::FloatTable(float min, float max, unsigned short divisions) : m_min(min), m_max(max)
	{
		m_contents = new float[divisions];
		float step = m_step = (max - min) / divisions, value = min;
		for(unsigned short a = 0; a < divisions; a++)
		{
			m_contents[a] = value;
			value += step;
		}
	}
	FloatTable::~FloatTable()
	{
		delete [] m_contents;
	}
	float FloatTable::GetValue(unsigned char idx)
	{
		return m_contents[idx];
	}
	float FloatTable::GetValue(unsigned short idx)
	{
		return m_contents[idx];
	}
	unsigned char FloatTable::GetByteIdx(float value)
	{
		if(value > m_max || value < m_min)
			return 0;
		return (unsigned char)((value - m_min) / m_step);
	}
}
