#ifndef CE_IMAGEARRAY_H
#define CE_IMAGEARRAY_H

//- Centhra Engine -
#include <CE/Image.h>
#include <CE/Rect.h>

namespace ce
{
	/* 	Image Array Class
	 * 	- Decompiles an image into an array of rectangles referencing the location of child images.
	 */
	class ImageArray
	{
		Image *m_image;
		Rect<float> **m_relativeArray;
		Rect<int> **m_array;
		unsigned int m_numColumns, *m_numRows;

		ImageArray();

	public:
		static ImageArray *createFromImage(Image *image);

		~ImageArray();

		Image *getImage() const;
		Rect<int> getRect(unsigned int column, unsigned int row) const;
		Rect<float> getRelativeRect(unsigned int column, unsigned int row) const;
	};
}

#endif
