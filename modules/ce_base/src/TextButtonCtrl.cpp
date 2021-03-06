#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/UI/TextButtonCtrl.h>
#include <CE/Renderer.h>
#include <CE/Base.h>

using namespace std;

//TODO: Use proper multiple-inheritance stuff for m_extent

namespace ce
{
	namespace ui
	{
		TextButtonCtrl::TextButtonCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font, const char *text, Color<float> color, Color<float> backgroundColor) : TextCtrl(position, extent, font, text, color), ButtonCtrl(position, extent), m_backgroundColor(backgroundColor)
		{
			ButtonCtrl::m_type = Type_TextButtonCtrl;
		}
		void TextButtonCtrl::DoRender(RenderContext &context)
		{
			//TODO: Update rendering
/*			glPushMatrix();
				glColor4fv(&m_backgroundColor[0]);
				glPushMatrix();
					glScalef((float)ButtonCtrl::m_extent[0], (float)ButtonCtrl::m_extent[1], 0.f);
					RenderSquare(context);
				glPopMatrix();
				TextCtrl::DoRender(context);
				glColor4ub(255, 255, 255, 255);
			glPopMatrix();
*/		}
		void TextButtonCtrl::SetBackgroundColor(Color<float> backgroundColor)
		{
			m_backgroundColor = backgroundColor;
		}
	}
}
