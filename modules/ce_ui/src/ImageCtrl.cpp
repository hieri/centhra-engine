//- Centhra Engine -
#include <CE/UI/ImageCtrl.h>
#include <CE/Base.h>

using namespace std;

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

namespace ce
{
	namespace ui
	{
		ImageCtrl::ImageCtrl(Vector2<int> position, Vector2<int> extent, Image *image,  Color color) : Control(position, extent)
		{
			m_color = color;
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
		void ImageCtrl::SetColor(Color color)
		{
			m_color = color;
		}
		Color ImageCtrl::GetColor() const
		{
			return m_color;
		}
		void ImageCtrl::DoRender()
		{
			Vector2<int> extent = GetExtent();
			glPushMatrix();
				glColor4ubv(&m_color[0]);
				glScalef((float)extent[0], (float)extent[1], 0.f);
				if(m_image)
				{
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
				}
				glColor4ub(255, 255, 255, 255);
			glPopMatrix();
		}
	}
}
