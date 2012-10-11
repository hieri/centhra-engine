#ifndef CE_CURSOR_H
#define CE_CURSOR_H

//- Standard Library -
#include <vector>

namespace ce
{
	/* 	Cursor Class
	 * 	- Acts as specialized handle for different cursors.
	 */
	class Cursor
	{
		static Cursor *ms_default[13];

		void *m_display, *m_handle;

		Cursor();
	
	public:
		static Cursor *getDefault(unsigned int type);

		enum Default
		{
			Normal,
			Wait,
			Text,
			Hand,
			SizeAll,
			SizeTopLeft,
			SizeTop,
			SizeTopRight,
			SizeLeft,
			SizeRight,
			SizeBottomLeft,
			SizeBottom,
			SizeBottomRight
		};

		void *getDisplay() const;
		void *getHandle() const;
	};
}

#endif
