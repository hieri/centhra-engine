#ifdef _MSC_VER
	#include <windows.h>
#else
	#include <X11/Xlib.h>
	#include <X11/cursorfont.h>
#endif

//- SDL -
#include <SDL_syswm.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Canvas.h>
#include <CE/Cursor.h>

// TODO: Add Mac support to the cursor changing.
// TODO: Add size cursors to the Linux-Based cursor list.
// TODO: Add custom cursor support.
// TODO: Fix minor bug related to cursor not resetting to default.
// TODO: Debug on whether or not the cursor changing method causes delay in the game.

using namespace std;

namespace ce
{
	Cursor *Cursor::ms_default[13];
	Cursor *Cursor::getDefault(unsigned int type)
	{
		if(ms_default[type])
			return ms_default[type];
		
		Cursor *cursor = new Cursor;

#ifdef _MSC_VER
		HCURSOR winCursor;

		switch(type)
		{
			case Normal:
				winCursor = LoadCursor(NULL, IDC_ARROW);
				break;
			case Wait:
				winCursor = LoadCursor(NULL, IDC_WAIT);
				break;
			case Text:
				winCursor = LoadCursor(NULL, IDC_IBEAM);
				break;
			case Hand:
				winCursor = LoadCursor(NULL, IDC_HAND);
				break;
			case SizeAll:
				winCursor = LoadCursor(NULL, IDC_SIZEALL);
				break;
			case SizeTopLeft:
				winCursor = LoadCursor(NULL, IDC_SIZENWSE);
				break;
			case SizeTop:
				winCursor = LoadCursor(NULL, IDC_SIZENS);
				break;
			case SizeTopRight:
				winCursor = LoadCursor(NULL, IDC_SIZENESW);
				break;
			case SizeLeft:
				winCursor = LoadCursor(NULL, IDC_SIZEWE);
				break;
			case SizeRight:
				winCursor = LoadCursor(NULL, IDC_SIZEWE);
				break;
			case SizeBottomLeft:
				winCursor = LoadCursor(NULL, IDC_SIZENESW);
				break;
			case SizeBottom:
				winCursor = LoadCursor(NULL, IDC_SIZENS);
				break;
			case SizeBottomRight:
				winCursor = LoadCursor(NULL, IDC_SIZENWSE);
				break;
			default:
				winCursor = LoadCursor(NULL, IDC_ARROW);
		}

	//	print("A %i %i\n", winCursor, type);
		cursor->m_handle = (void *)winCursor;
#else
		XID x11Cursor;
		Display *x11Display;

		x11Display = XOpenDisplay(NULL);

		switch(type)
		{
			case Normal:
				x11Cursor = XCreateFontCursor(x11Display, XC_left_ptr);
				break;
			case Wait:
				x11Cursor = XCreateFontCursor(x11Display, XC_watch);
				break;
			case Text:
				x11Cursor = XCreateFontCursor(x11Display, XC_xterm);
				break;
			case Hand:
				x11Cursor = XCreateFontCursor(x11Display, XC_hand1);
				break;
			case SizeAll:
				x11Cursor = XCreateFontCursor(x11Display, XC_cross);
				break;
			case SizeTopLeft:
				x11Cursor = XCreateFontCursor(x11Display, XC_top_left_corner);
				break;
			case SizeTop:
				x11Cursor = XCreateFontCursor(x11Display, XC_top_side);
				break;
			case SizeTopRight:
				x11Cursor = XCreateFontCursor(x11Display, XC_top_right_corner);
				break;
			case SizeLeft:
				x11Cursor = XCreateFontCursor(x11Display, XC_left_side);
				break;
			case SizeRight:
				x11Cursor = XCreateFontCursor(x11Display, XC_right_side);
				break;
			case SizeBottomLeft:
				x11Cursor = XCreateFontCursor(x11Display, XC_bottom_left_corner);
				break;
			case SizeBottom:
				x11Cursor = XCreateFontCursor(x11Display, XC_bottom_side);
				break;
			case SizeBottomRight:
				x11Cursor = XCreateFontCursor(x11Display, XC_bottom_right_corner);
				break;
			default:
				x11Cursor = XCreateFontCursor(x11Display, XC_left_ptr);
		}

		cursor->m_handle = (void *)x11Cursor;
		cursor->m_display = (void *)x11Display;
#endif

		ms_default[type] = cursor;
		return cursor;
	}
	Cursor::Cursor()
	{
		m_display = 0;
		m_handle = 0;
	}
	void *Cursor::getDisplay() const
	{
		return m_display;
	}
	void *Cursor::getHandle() const
	{
		return m_handle;
	}
}
