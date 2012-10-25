#ifndef _CE_EVENT_H_
#define _CE_EVENT_H_

//- Centhra Engine -
#include <CE/ConfigFrontend.h>

//- TODO: Incorporate event stack, with peek/wait functionality. -

namespace ce
{
	class Canvas;

	typedef enum EventType
	{
		Null,
		KeyDown,
		KeyUp,
		MouseButtonDown,
		MouseButtonUp,
		MouseMotion
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
		unsigned int keyCode, state;
	} KeyEvent;

	typedef struct MouseButtonEvent
	{
		int type;
		Canvas *canvas;
		int x, y;
		unsigned int button, state;
	} MouseButtonEvent;

	typedef struct MouseMotionEvent
	{
		int type;
		Canvas *canvas;
		int x, y;
	} MouseMotionEvent;

	typedef union Event
	{
		int type;
		DefaultEvent base;
		KeyEvent key;
		MouseButtonEvent mouseButton;
		MouseMotionEvent mouseMotion;
		char padding[32];
	} Event;
}

#endif
