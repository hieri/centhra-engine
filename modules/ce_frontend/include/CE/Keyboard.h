#ifndef _CE_KEYBOARD_H_
#define _CE_KEYBOARD_H_

//- Centhra Engine -
#include <CE/KeyCode.h>

namespace ce
{
	unsigned int NativeScanCodeToKeyCode(unsigned int nativeScanCode);
	unsigned int NativeScanCodeToScanCode(unsigned int nativeScanCode);
	unsigned int ScanCodeToKeyCode(unsigned int scanCode);
}

#endif
