#ifndef _CE_EVENT_H_
#define _CE_EVENT_H_

//- Centhra Engine -
#include <CE/ConfigFrontend.h>

namespace ce
{
	typedef enum EventType
	{
		Null,
		KeyDown,
		KeyUp
	} EventType;

	typedef struct KeyEvent
	{
		int type;
		unsigned int keyCode;
		unsigned int state;
	} KeyEvent;

	typedef union Event
	{
		int type;

		KeyEvent key;
		char padding[12];
	} Event;
}

#endif
