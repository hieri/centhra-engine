#ifndef _CE_UI_TEXTCTRL_H_
#define _CE_UI_TEXTCTRL_H_

//- Standard Library -
#include <string>

//- Centhra Engine -
#include <CE/UI/ColorCtrl.h>
#include <CE/Font.h>

namespace ce
{
	namespace ui
	{
		class TextCtrl : public ColorCtrl
		{
		protected:
			std::string m_text;
			Font *m_font;

			virtual void DoRender();

		public:
			TextCtrl(Vector2<int> position, Vector2<int> extent, Font *font, const char *text = "", Color color = Color());
			
			Font *GetFont() const;
			void SetFont(Font *font);
			const char *GetText() const;
			void SetText(const char *text);
		};
	}
}

#endif
