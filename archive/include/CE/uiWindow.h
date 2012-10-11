#ifndef CE_UIWINDOW_H
#define CE_UIWINDOW_H

//- Standard Library -
#include <string>

//- Centhra Engine -
#include <CE/Color.h>
#include <CE/Font.h>
#include <CE/Image.h>
#include <CE/ImageArray.h>
#include <CE/uiEntity.h>
#include <CE/Vector2.h>

namespace ce
{
	/* 	UI Window Class
	 * 	- Displays a Window based on a uiWindowTemplate.
	 */
	class uiWindow : public uiEntity
	{
		bool m_canClose, m_canMaximize, m_canMinimize, m_canMove, m_canResize;
		std::string m_title;

	protected:
		void draw();

	public:
		class Template
		{
			Color m_titleColor;
			Font *m_font;
			ImageArray *m_skin;
			unsigned int m_edgeSize, m_titleHeight, m_titleTextSize;
			Vector2<int> m_titlePosition, m_minimumSize;

		public:
			Template();

			unsigned int getEdgeSize() const;
			Font *getFont() const;
			Vector2<int> getMinimumSize() const;
			ImageArray *getSkin() const;
			Color getTitleColor() const;
			unsigned int getTitleHeight() const;
			Vector2<int> getTitlePosition() const;
			unsigned int getTitleTextSize() const;
			void setEdgeSize(unsigned int edgeSize);
			void setFont(Font *font);
			void setMinimumSize(Vector2<int> minimumSize);
			void setSkin(ImageArray *skin);
			void setTitleColor(Color titleColor);
			void setTitleHeight(unsigned int titleHeight);
			void setTitlePosition(Vector2<int> position);
			void setTitleTextSize(unsigned int titleTextSize);
		};

		uiWindow(uiInstance *ui, Template *t);

		bool canClose() const;
		bool canMaximize() const;
		bool canMinimize() const;
		bool canMove() const;
		bool canResize()const;
		std::string getTitle() const;
		void onMouseButtonPressed(unsigned char button, int x, int y);
		void onMouseButtonReleased(unsigned char button, int x, int y);
		void onMouseMoved(int x, int y, int deltaX, int deltaY);
		void onMouseOver(int x, int y);
		void setCanClose(bool canClose);
		void setCanMaximize(bool canMaximize);
		void setCanMinimize(bool canMinimize);
		void setCanMove(bool canMove);
		void setCanResize(bool canResize);
		void setTitle(std::string title);

	private:
		Template *m_template;
	};
}

#endif
