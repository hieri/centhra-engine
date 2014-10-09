#ifndef _CE_UI_TEXTBUTTONCTRL_H_
#define _CE_UI_TEXTBUTTONCTRL_H_

//- Centhra Engine -
#include <CE/UI/ButtonCtrl.h>
#include <CE/UI/TextCtrl.h>

namespace ce
{
	namespace ui
	{
		class TextButtonCtrl : public TextCtrl, public ButtonCtrl
		{
		protected:
			Color m_backgroundColor;

			virtual void DoRender();

		public:
			TextButtonCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font, const char *text = "", Color color = Color(), Color backgroundColor = Color());

			void SetBackgroundColor(Color backgroundColor);
		};
	}
}

#endif
