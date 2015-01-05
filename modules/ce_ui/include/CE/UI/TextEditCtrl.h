#ifndef _CE_UI_TEXTEDITCTRL_H_
#define _CE_UI_TEXTEDITCTRL_H_

//- Centhra Engine -
#include <CE/UI/TextCtrl.h>

namespace ce
{
	namespace ui
	{
		class TextEditCtrl : public TextCtrl
		{
			//- Temp Vars - Will be removed with abstraction -
			bool m_leftShift, m_rightShift;
			unsigned short m_maxSize;

		protected:
			virtual void DoRender(RenderContext &context);

		public:
			TextEditCtrl(Vector2<short> position, Vector2<short> extent, Font *font, unsigned short maxSize, const char *text = "", Color color = Color());
			virtual ~TextEditCtrl();

			virtual bool OnEvent(Event &event);
		};
	}
}

#endif
