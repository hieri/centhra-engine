#ifndef _CE_UI_IMAGECTRL_H_
#define _CE_UI_IMAGECTRL_H_

//- Centhra Engine -
#include <CE/UI/Control.h>
#include <CE/Color.h>
#include <CE/Image.h>

namespace ce
{
	namespace ui
	{
		class ImageCtrl : public Control
		{
			Color m_color;
			Image *m_image;

		protected:
			virtual void DoRender();

		public:
			ImageCtrl(Vector2<int> position, Vector2<int> extent, Image *image, Color color = Color());
			
			Color GetColor() const;
			void SetColor(Color color);
			Image *GetImage() const;
			void SetImage(Image *image);
		};
	}
}

#endif
