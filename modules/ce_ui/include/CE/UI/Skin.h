#ifndef _CE_UI_SKIN_H_
#define _CE_UI_SKIN_H_

//- Centhra Engine -
#include <CE/Image.h>
#include <CE/Rect.h>

namespace ce
{
	namespace ui
	{
		class Skin
		{
			Rect<float> **m_relativeArray;
			Rect<int> **m_array;
			Image *m_image;
			unsigned int m_numRows, *m_numColumns;

		public:
			Skin(Image *image);
			~Skin();

			Image *GetImage() const;
			Rect<int> GetRect(unsigned int column, unsigned int row) const;
			Rect<float> GetRelativeRect(unsigned int column, unsigned int row) const;
		};
	}
}

#endif
