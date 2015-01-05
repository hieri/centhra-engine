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
		class ColorCtrl : public Control
		{
		protected:
			Color m_color;

			virtual void DoRender(RenderContext &context);

		public:
			ColorCtrl(Vector2<short> position, Vector2<short> extent, Color color = Color());
			
			Color GetColor() const;
			void SetColor(Color color);
		};
	}
}

#endif
