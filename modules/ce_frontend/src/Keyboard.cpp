//- Centhra Engine -
#include <CE/Keyboard.h>

namespace ce
{
#ifdef _WIN32
	unsigned int g_windowsScanCodeTable [] =
	{
		Scan_Unknown,
		Scan_Escape,
		Scan_1,
		Scan_2,
		Scan_3,
		Scan_4,
		Scan_5,
		Scan_6,

		Scan_7,
		Scan_8,
		Scan_9,
		Scan_0,
		Scan_Minus,
		Scan_Equal,
		Scan_Backspace,
		Scan_Tab,

		Scan_Q,
		Scan_W,
		Scan_E,
		Scan_R,
		Scan_T,
		Scan_Y,
		Scan_U,
		Scan_I,

		Scan_O,
		Scan_P,
		Scan_BracketLeft,
		Scan_BracketRight,
		Scan_Return,
		Scan_ControlLeft,
		Scan_A,
		Scan_S,

		Scan_D,
		Scan_F,
		Scan_G,
		Scan_H,
		Scan_J,
		Scan_K,
		Scan_L,
		Scan_Semicolon,

		Scan_Apostrophe,
		Scan_Grave,
		Scan_ShiftLeft,
		Scan_Backslash,
		Scan_Z,
		Scan_X,
		Scan_C,
		Scan_V,

		Scan_B,
		Scan_N,
		Scan_M,
		Scan_Comma,
		Scan_Period,
		Scan_Slash,
		Scan_ShiftRight,
		Scan_PrintScreen,
	
		Scan_AltLeft,
		Scan_Space,
		Scan_CapsLock,
		Scan_F1,
		Scan_F2,
		Scan_F3,
		Scan_F4,
		Scan_F5,

		Scan_F6,
		Scan_F7,
		Scan_F8,
		Scan_F9,
		Scan_F10,
		Scan_NumLock,
		Scan_ScrollLock,
		Scan_Home,

		Scan_Up,
		Scan_PageUp,
		Scan_KP_Minus,
		Scan_Left,
		Scan_KP_5,
		Scan_Right,
		Scan_KP_Plus,
		Scan_End,

		Scan_Down,
		Scan_PageDown,
		Scan_Insert,
		Scan_Delete,
		Scan_Unknown,
		Scan_Unknown,
		Scan_NUS_Backslash,
		Scan_F12,

		Scan_Pause,
		Scan_Unknown,
		Scan_GUILeft,
		Scan_GUIRight,
		Scan_Application,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,

		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,

		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,

		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown,
		Scan_Unknown
	};

	unsigned int ConvertNativeScanCode(unsigned long scanCode)
	{
		return 0;
	}
#endif
}
