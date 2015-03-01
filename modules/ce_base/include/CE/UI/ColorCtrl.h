#ifndef _CE_UI_COLORCTRL_H_
#define _CE_UI_COLORCTRL_H_

//- Standard Library -
#include <string>

//- Centhra Engine -
#include <CE/UI/Control.h>
#include <CE/Color.h>

namespace ce
{
	namespace ui
	{
		class IBackgroundColor
		{
		public:
			IBackgroundColor(Color<float> color);
			virtual ~IBackgroundColor();

			//- Background Color -
		protected:
			Color<float> m_backgroundColor;
		public:
			Color<float> GetBackgroundColor() const;
			void SetBackgroundColor(Color<float> color);
		};

		class ColorCtrl : public Control, public IBackgroundColor
		{
		public:
			ColorCtrl(Vector2<short> position, Vector2<short> extent, Color<float> backgroundColor = Color<float>(1.f, 1.f, 1.f, 1.f));

		protected:
			virtual void DoRender(RenderContext &context);
		};
	}
}

#endif
