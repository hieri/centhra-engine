#ifndef CE_UIRECT_H
#define CE_UIRECT_H

//- Centhra Engine -
#include <CE/Color.h>
#include <CE/uiEntity.h>

namespace ce
{
	/* 	UI Rectangle Class
	 * 	- Displays a colored rectangle.
	 */
	class uiRect : public uiEntity
	{
		Color m_color;

	protected:
		void draw();

	public:
		uiRect(uiInstance *ui);

		Color getColor() const;
		void setColor(Color color);
	};
}

#endif
