#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/UI/TextButtonCtrl.h>
#include <CE/RenderPrimitives.h>
#include <CE/Base.h>

using namespace std;

//TODO: Use proper multiple-inheritance stuff for m_extent

namespace ce
{
	namespace ui
	{
		TextButtonCtrl::TextButtonCtrl(Vector2<int> position, Vector2<int> extent, Font *font, const char *text, Color color) : TextCtrl(position, extent, font, text, color), ButtonCtrl(position, extent)
		{
		}
		void TextButtonCtrl::DoRender()
		{
			TextCtrl::DoRender();
			return;
			glPushMatrix();
				glColor4ub(255, 255, 0, 255);
				glPushMatrix();
					glScalef((float)ButtonCtrl::m_extent[0], (float)ButtonCtrl::m_extent[1], 0.f);
					RenderSquare();
				glPopMatrix();
				TextCtrl::DoRender(); //???????????????????????/ debugging sucks without a proper debugger ;]
				glColor4ub(255, 255, 255, 255);
			glPopMatrix();
		}
	}
}
