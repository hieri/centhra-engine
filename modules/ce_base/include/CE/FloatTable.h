#ifndef _CE_FLOAT_TABLE_H_
#define _CE_FLOAT_TABLE_H_

namespace ce
{
	class FloatTable
	{
		float *m_contents, m_min, m_max, m_step;
		unsigned short m_divisions;

	public:
		FloatTable(float min, float max, unsigned short divisions);
		~FloatTable();

		float GetValue(unsigned char idx);
		float GetValue(unsigned short idx);
		unsigned char GetByteIdx(float value);
	};
}

#endif
