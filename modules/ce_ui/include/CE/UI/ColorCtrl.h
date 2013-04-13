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
			Color m_color;

		protected:
			virtual void DoRender();

		public:
			ColorCtrl(Vector2<int> position, Vector2<int> extent, Color color = Color());
			
			Color GetColor() const;
			void SetColor(Color color);
		};
	}
}

#endif
