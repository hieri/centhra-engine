#ifndef _CE_EVENT_H_
#define _CE_EVENT_H_

//- Centhra Engine -
#include <CE/ConfigFrontend.h>
#include <CE/KeyCode.h>

//TODO: incorporate event stack, with peek/wait functionality

namespace ce
{
	class Canvas;

	namespace event
	{
		typedef enum EventType
		{
			Null,
			PreRender,
			Render,
			PostRender,
			KeyDown,
			KeyUp,
			MouseButtonDown,
			MouseButtonUp,
			MouseMotion,
			MouseScroll,
			WindowResize
		} EventType;

		typedef enum EventMask
		{
			Base_Mask = 0,
			Mask_PreRender          = 1 << (Base_Mask + 0),
			Mask_Render             = 1 << (Base_Mask + 1),
			Mask_PostRender         = 1 << (Base_Mask + 2),
			Mask_KeyDown            = 1 << (Base_Mask + 3),
			Mask_KeyUp              = 1 << (Base_Mask + 4),
			Mask_MouseButtonDown    = 1 << (Base_Mask + 5),
			Mask_MouseButtonUp      = 1 << (Base_Mask + 6),
			Mask_MouseMotion        = 1 << (Base_Mask + 7),
			Mask_MouseScroll         = 1 << (Base_Mask + 8),
			Mask_WindowResize       = 1 << (Base_Mask + 9)
		} EventMask;

		typedef enum MouseButtonType
		{
			Unknown,
			MouseButtonLeft,
			MouseButtonRight,
			MouseButtonMiddle
		} MouseButtonType;
	}

	/**	@brief Base Event Data Structure Class
	 */
	typedef struct BaseEvent
	{
		unsigned char type;
		unsigned short mask;
		unsigned long long timeMS;
		Canvas *canvas;
	} BaseEvent;

	/**	@brief Render Event Data Structure Class
	 */
	typedef struct RenderEvent : BaseEvent
	{
	} RenderEvent;

	/**	@brief Key Event Data Structure Class
	 */
	typedef struct KeyEvent : BaseEvent
	{
		unsigned char scanCode;
		unsigned int keyCode, state;
	} KeyEvent;

	//------------------------ Mouse Events ------------------------

	/**	@brief Base Mouse Event Data Structure Class
	*/
	typedef struct BaseMouseEvent : BaseEvent
	{
		int_canvas x, y;
	} BaseMouseEvent;

	/**	@brief Mouse Button Event Data Structure Class
	 */
	typedef struct MouseButtonEvent : BaseMouseEvent
	{
		unsigned int button, state;
	} MouseButtonEvent;

	/**	@brief Mouse Scroll Event Data Structure Class
	 */
	typedef struct MouseScrollEvent : BaseMouseEvent
	{
		int delta;
		bool isHorizontal;
	} MouseScrollEvent;

	//------------------------ Window Events ------------------------

	/**	@brief Window Resize Event Data Structure Class
	 */
	typedef struct WindowResizeEvent : BaseEvent
	{
		int_canvas width, height;
	} WindowResizeEvent;

	/**	@brief Event Union Data Structure Class
	 */
	typedef union Event
	{
		unsigned char type;
		BaseEvent base;
		RenderEvent render;
		KeyEvent key;
		BaseMouseEvent mouse;
		MouseButtonEvent mouseButton;
		BaseMouseEvent mouseMotion;
		MouseScrollEvent mouseScroll;
		WindowResizeEvent windowResize;
		char padding[64];
	} Event;
}

#endif
