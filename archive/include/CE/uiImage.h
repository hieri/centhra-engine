#ifndef CE_UIIMAGE_H
#define CE_UIIMAGE_H

//- Centhra Engine -
#include <CE/Image.h>
#include <CE/uiRect.h>

namespace ce
{
	/* 	UI Image Class
	 * 	- Displays an Image.
	 */
	class uiImage : public uiRect
	{
		Image *m_image;

	protected:
		void draw();

	public:
		uiImage(uiInstance *ui);
		
		Image *getImage() const;
		void setImage(Image *img);
	};
}

#endif
