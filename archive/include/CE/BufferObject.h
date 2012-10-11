#ifndef CE_BUFFEROBJECT_H
#define CE_BUFFEROBJECT_H

//- Standard Library -
#include <string>

namespace ce
{
	class BufferObject
	{
		int m_type;
		unsigned int m_glBufferObject;

	public:
		static void unbind(int type);

		enum Modes
		{
			StaticDraw = 0x88E4
		};
		enum Types
		{
			Array = 0x8892,
			ElementArray = 0x8893
		};

		BufferObject(int type);
		~BufferObject();

		void bind();
		void setData(void *data, unsigned int size, int mode);
	};
}

#endif
