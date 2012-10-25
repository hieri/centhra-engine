#ifndef _CE_EVENT_H_
#define _CE_EVENT_H_

//- Centhra Engine -
#include <CE/ConfigFrontend.h>

namespace ce
{
	class Canvas;

	typedef enum EventType
	{
		Null,
		KeyDown,
		KeyUp
	} EventType;

	typedef struct DefaultEvent
	{
		int type;
		Canvas *canvas;
	} DefaultEvent;

	typedef struct KeyEvent
	{
		int type;
		Canvas *canvas;
		unsigned int keyCode;
		unsigned int state;
	} KeyEvent;

	typedef union Event
	{
		int type;
		DefaultEvent base;
		KeyEvent key;
		char padding[20];
	} Event;
}

#endif
