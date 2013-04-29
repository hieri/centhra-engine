#ifndef _CE_UI_IMAGECTRL_H_
#define _CE_UI_IMAGECTRL_H_

//- Centhra Engine -
#include <CE/UI/ColorCtrl.h>
#include <CE/Image.h>

namespace ce
{
	namespace ui
	{
		class ImageCtrl : public ColorCtrl
		{
		protected:
			Image *m_image;

			virtual void DoRender();

		public:
			ImageCtrl(Vector2<int> position, Vector2<int> extent, Image *image, Color color = Color());
			
			Image *GetImage() const;
			void SetImage(Image *image);
		};
	}
}

#endif
