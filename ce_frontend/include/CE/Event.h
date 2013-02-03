#ifndef _CE_EVENT_H_
#define _CE_EVENT_H_

//- Centhra Engine -
#include <CE/ConfigFrontend.h>

//- TODO: incorporate event stack, with peek/wait functionality -

namespace ce
{
	class Canvas;

	typedef enum EventType
	{
		Null,
		PreRender,
		PostRender,
		KeyDown,
		KeyUp,
		MouseButtonDown,
		MouseButtonUp,
		MouseMotion
	} EventType;

	/**	@brief Default Event Data Structure
	 */
	typedef struct DefaultEvent
	{
		int type;
		unsigned long timeMS;
		Canvas *canvas;
	} DefaultEvent;

	/**	@brief Render Event Data Structure
	 */
	typedef struct RenderEvent
	{
		int type;
		unsigned long timeMS;
		Canvas *canvas;
	} RenderEvent;

	/**	@brief Key Event Data Structure
	 */
	typedef struct KeyEvent
	{
		int type;
		unsigned long timeMS;
		Canvas *canvas;
		unsigned int keyCode, state;
	} KeyEvent;

	/**	@brief Mouse Button Event Data Structure
	 */
	typedef struct MouseButtonEvent
	{
		int type;
		unsigned long timeMS;
		Canvas *canvas;
		int x, y;
		unsigned int button, state;
	} MouseButtonEvent;

	/**	@brief Mouse Motion Event Data Structure
	 */
	typedef struct MouseMotionEvent
	{
		int type;
		unsigned long timeMS;
		Canvas *canvas;
		int x, y;
	} MouseMotionEvent;

	/**	@brief Event Union Data Structure
	 */
	typedef union Event
	{
		int type;
		DefaultEvent base;
		RenderEvent render;
		KeyEvent key;
		MouseButtonEvent mouseButton;
		MouseMotionEvent mouseMotion;
		char padding[32];
	} Event;
}

#endif
