#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/UI/ImageCtrl.h>
#include <CE/Base.h>

using namespace std;

namespace ce
{
	namespace ui
	{
		ImageCtrl::ImageCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Image *image, Color color) : ColorCtrl(position, extent, color)
		{
			m_type = Type_ImageCtrl;
			m_image = image;
		}
		void ImageCtrl::SetImage(Image *image)
		{
			m_image = image;
		}
		Image *ImageCtrl::GetImage() const
		{
			return m_image;
		}
		void ImageCtrl::DoRender(RenderContext &context)
		{
			glPushMatrix();
				glColor4ubv(&m_color[0]);
				glScalef((float)m_extent[0], (float)m_extent[1], 0.f);
				if(m_image)
				{
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glEnable(GL_TEXTURE_2D);
						m_image->Bind();
						glBegin(GL_QUADS);
							glTexCoord2i(0, 1);
							glVertex2i(0, 0);

							glTexCoord2i(1, 1);
							glVertex2i(1, 0);

							glTexCoord2i(1, 0);
							glVertex2i(1, 1);

							glTexCoord2i(0, 0);
							glVertex2i(0, 1);
						glEnd();
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_BLEND);
				}
				glColor4ub(255, 255, 255, 255);
			glPopMatrix();
		}
	}
}
