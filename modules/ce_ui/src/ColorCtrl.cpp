#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/UI/ColorCtrl.h>
#include <CE/Renderer.h>
#include <CE/Base.h>

using namespace std;

namespace ce
{
	namespace ui
	{
		ColorCtrl::ColorCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Color color) : Control(position, extent)
		{
			m_type = Type_ColorCtrl;
			m_color = color;
		}
		void ColorCtrl::SetColor(Color color)
		{
			m_color = color;
		}
		Color ColorCtrl::GetColor() const
		{
			return m_color;
		}
		void ColorCtrl::DoRender(RenderContext &context)
		{
/*			ShaderProgram *genericProgram = 0;
			if(context.useShaders)
				genericProgram = ShaderProgram::GetGenericProgram();
			if(genericProgram)
			{
				Matrix4x4<float> mvpMatrix = context.projectionMatrix * m_absoluteMatrix;
				//-------------------------- OpenGL 2.1 w/ GLSL 1.2 --------------------------
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				genericProgram->Use();
					int mvpMatrixLoc = genericProgram->GetUniformLocation("mvpMatrix");
					glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, &mvpMatrix[0]);
					int colorLoc = genericProgram->GetUniformLocation("color");
					//TODO: Switch to float based coloring??
					glUniform4f(colorLoc, (float)m_color[0] / 255.f, (float)m_color[1] / 255.f, (float)m_color[2] / 255.f, (float)m_color[3] / 255.f);
					RenderSquare(context);
				genericProgram->Unuse();

				glDisable(GL_BLEND);
			}
			else*/
			{
				//-------------------------- OpenGL 1.0 --------------------------
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glColor4ub(m_color[0], m_color[1], m_color[2], m_color[3]);

				glLoadMatrixf(&m_absoluteMatrix[0]);
				RenderSquare(context);
				glColor3ub(255, 255, 255);

				glDisable(GL_BLEND);
			}
		}
	}
}
