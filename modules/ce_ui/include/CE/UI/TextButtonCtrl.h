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
			Color<float> m_backgroundColor;

			virtual void DoRender(RenderContext &context);

		public:
			TextButtonCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font, const char *text = "", Color<float> color = Color<float>(1.f, 1.f, 1.f, 1.f), Color<float> backgroundColor = Color<float>(0.f, 0.f, 0.f, 1.f));

			void SetBackgroundColor(Color<float> backgroundColor);
		};
	}
}

#endif
