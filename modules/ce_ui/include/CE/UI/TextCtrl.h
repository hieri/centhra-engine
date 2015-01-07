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
			TextCtrl(Vector2<short> position, Vector2<short> extent, Font *font, const char *text = "", Color<float> textColor = Color<float>(1.f, 1.f, 1.f, 1.f), Color<float> backgroundColor = Color<float>(0.f, 0.f, 0.f, 0.f));
			
			Font *GetFont() const;
			void SetFont(Font *font);
			const char *GetText() const;
			void SetText(const char *format, ...);

			//- Text Color -
		protected:
			Color<float> m_textColor;
		public:
			Color<float> GetTextColor() const;
			void SetTextColor(Color<float> textColor);

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
