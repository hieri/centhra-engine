#ifndef _CE_UI_TEXTDROPDOWNCTRL_H_
#define _CE_UI_TEXTDROPDOWNCTRL_H_

//- Standard Library -
#include <vector>
#include <string>

//- Centhra Engine -
#include <CE/UI/TextCtrl.h>

namespace ce
{
	namespace ui
	{
		class TextDropDownCtrl : public TextCtrl
		{
			class TextDropDownSelectCtrl : public ColorCtrl
			{
			protected:
				TextDropDownCtrl *m_source;
				Font *m_font;
				unsigned short m_hoverIdx;

				virtual void DoRender();
	
				friend class TextDropDownCtrl;

			public:
				TextDropDownSelectCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font, TextDropDownCtrl *source, Color color = Color(0, 0, 0, 0));
				~TextDropDownSelectCtrl();

				virtual bool OnEvent(Event &event);

				virtual void OnControlZoneSelect(ControlZone *zone);
				virtual void OnControlZoneHover(ControlZone *zone);

				void UpdateControlZones();
			};

			friend class TextDropDownSelectCtrl;

		protected:
			TextDropDownSelectCtrl *m_selector;
			int_canvas m_selectorWidth;
			std::string m_placeHolder;

			virtual void DoRender();

		public:
			TextDropDownCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font, int_canvas selectorWidth, const char *text = "", Color color = Color());
			~TextDropDownCtrl();

			void ReattachSelector();

			virtual void OnAdded(Control *parent);
			virtual void OnDimensionUpdate();
			virtual bool OnEvent(Event &event);

			//- Selections -
		protected:
			unsigned short m_selectionIdx;
			std::vector<std::pair<unsigned char, std::string> > m_selections;
			bool(*m_OnSelection)(TextDropDownCtrl *ctrl, unsigned char id);
		public:
			void AddSelection(unsigned char id, std::string text);
			void ClearSelections();
			void Select(unsigned char id);
			void SetOnSelection(bool(*callback)(TextDropDownCtrl *, unsigned char id));
		};
	}
}

#endif
