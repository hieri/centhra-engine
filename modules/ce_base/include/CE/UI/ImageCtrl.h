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

			virtual void DoRender(RenderContext &context);

		public:
			ImageCtrl(Vector2<short> position, Vector2<short> extent, Image *image, Color<float> color = Color<float>(1.f, 1.f, 1.f, 1.f));
			
			Image *GetImage() const;
			void SetImage(Image *image);
		};
	}
}

#endif
