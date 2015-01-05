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

			virtual void DoRender(RenderContext &context);

		public:
			TextCtrl(Vector2<short> position, Vector2<short> extent, Font *font, const char *text = "", Color color = Color());
			
			Font *GetFont() const;
			void SetFont(Font *font);
			const char *GetText() const;
			void SetText(const char *format, ...);

			//- Text Wrap -
		protected:
			bool m_isWrapping;
			void ApplyWrap();
		public:
			bool IsWrapping() const;
			void SetWrapping(bool wrapping);
		};
	}
}

#endif
