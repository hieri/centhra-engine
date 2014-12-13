#ifndef _CE_UI_BUTTONCTRL_H_
#define _CE_UI_BUTTONCTRL_H_

//- Standard Library -
#include <map>
#include <vector>

//- Centhra Engine -
#include <CE/UI/Control.h>

namespace ce
{
	namespace ui
	{
		// Button States (Toggle)
		// Buttons with images for each state
		// Buttons with text
		// Callbacks: onPressed, onReleased

		class ButtonCtrl : public Control
		{
			static std::map<int, std::vector<ButtonCtrl *> *> ms_buttonGroupMap;

			bool m_isEnabled;
			int m_group;

			bool (*m_OnButtonDown)(ButtonCtrl *ctrl);
			bool (*m_OnButtonUp)(ButtonCtrl *ctrl);

		protected:
			virtual void DoRender();

		public:
			ButtonCtrl(Vector2<short> position, Vector2<short> extent);
			~ButtonCtrl();

			int GetGroup() const;
			bool IsEnabled() const;
			void SetOnButtonDown(bool (*callback)(ButtonCtrl *));
			void SetOnButtonUp(bool (*callback)(ButtonCtrl *));
			void SetEnabled(bool enabled);
			void SetGroup(int group);

			virtual bool OnEvent(Event &event);
		};
	}
}

#endif
