#ifndef _CE_UI_SCROLLCTRL_H_
#define _CE_UI_SCROLLCTRL_H_

//- Standard Library -
#include <string>

//- Centhra Engine -
#include <CE/UI/Control.h>
#include <CE/UI/Skin.h>

namespace ce
{
	namespace ui
	{
		class ScrollCtrl : public Control
		{
		protected:
			Skin *m_skin;
			Vector2<int_canvas> m_scrollExtent;
			Vector2<float> m_scrollPercentage;
			bool m_horizontalScroll, m_verticalScroll;

			virtual void DoRender();
			virtual void DoOverlay();

			void Scroll(Vector2<int_canvas> amt);

		public:
			ScrollCtrl(Vector2<short> position, Vector2<short> extent, Skin *skin);

			void UpdateScroll();

			virtual void OnMemberAdded(Control *ctrl);
			virtual void OnMemberRemoved(Control *ctrl);

			virtual bool OnEvent(Event &event);
		};
	}
}

#endif
