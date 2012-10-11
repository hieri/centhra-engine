#ifndef CE_UITEXT_H
#define CE_UITEXT_H

//- Standard Library -
#include <string>

//- Centhra Engine -
#include <CE/Font.h>
#include <CE/uiEntity.h>

namespace ce
{
	/* 	UI Text Class
	 * 	- Displays text.
	 */
	class uiText : public uiEntity
	{
		Font *m_font;
		std::string m_text;
		unsigned int m_textSize;

	protected:
		void draw();

	public:
		uiText(uiInstance *ui);

		Font *getFont() const;
		std::string getText() const;
		unsigned int getTextSize() const;
		void setFont(Font *f);
		void setText(const char *format, ...);
		void setText(std::string text);
		void setTextSize(unsigned int textSize);
	};
}

#endif
