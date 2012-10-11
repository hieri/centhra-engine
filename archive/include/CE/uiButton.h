#ifndef CE_UIBUTTON_H
#define CE_UIBUTTON_H

//- Standard Library -
#include <string>

//- Centhra Engine -
#include <CE/Font.h>
#include <CE/ImageArray.h>
#include <CE/uiEntity.h>

namespace ce
{
	/* 	UI Button Class
	 * 	- Emulates a Button.
	 */
	class uiButton : public uiEntity
	{
		bool m_activated;
		std::string m_text;
		char m_state;

	protected:
		void draw();

	public:
		enum States
		{
			Idle,
			Activated,
			Disabled,
			DisabledActivated,
			Hover,
			Press
		};

		class Template
		{
			bool m_isToggle;
			Color m_textColor;
			Font *m_font;
			ImageArray *m_skin;
			unsigned int m_textSize;
			Vector2<float> m_textOffset;

		public:
			Template();

			Font *getFont() const;
			ImageArray *getSkin() const;
			Color getTextColor() const;
			Vector2<float> getTextOffset() const;
			unsigned int getTextSize() const;
			bool isToggle() const;
			void setFont(Font *font);
			void setSkin(ImageArray *skin);
			void setTextColor(Color textColor);
			void setTextOffset(Vector2<float> offset);
			void setTextSize(unsigned int textSize);
			void setToggle(bool isToggle);
		};

		uiButton(uiInstance *ui, Template *t);
		std::string getText() const;
		void onMouseButtonPressed(unsigned char button, int x, int y);
		void onMouseButtonReleased(unsigned char button, int x, int y);
		void onMouseEnter(int x, int y);
		void onMouseLeave(int x, int y);
		void onMouseMoved(int x, int y, int deltaX, int deltaY);
		void onMouseOver(int x, int y);
		void setText(std::string text);

	private:
		Template *m_template;
	};
}

#endif
