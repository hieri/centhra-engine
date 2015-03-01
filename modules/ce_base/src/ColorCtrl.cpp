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
		//---------------------------- IBackgroundColor ----------------------------
		IBackgroundColor::IBackgroundColor(Color<float> color) : m_backgroundColor(color)
		{
		}
		IBackgroundColor::~IBackgroundColor()
		{
		}
		void IBackgroundColor::SetBackgroundColor(Color<float> color)
		{
			m_backgroundColor = color;
		}
		Color<float> IBackgroundColor::GetBackgroundColor() const
		{
			return m_backgroundColor;
		}

		//---------------------------- ColorCtrl ----------------------------
		ColorCtrl::ColorCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Color<float> backgroundColor) : Control(position, extent), IBackgroundColor(backgroundColor)
		{
			m_type = Type_ColorCtrl;
		}
		void ColorCtrl::DoRender(RenderContext &context)
		{
			ConfigGeneric();
			ShaderProgram::GenericProgram *program = 0;
			if(context.useShaders)
				program = UseGenericProgram();
			if(program != 0)
			{
				Matrix4x4<float> mvpMatrix = context.projectionMatrix * m_absoluteMatrix;
				mvpMatrix *= Matrix4x4<float>::BuildFromScale(m_extent);
				glUniformMatrix4fv(program->mvpMatrix, 1, GL_FALSE, &mvpMatrix[0]);
				glUniform4fv(program->color, 1, &m_backgroundColor[0]);
			}
			else
			{
				Matrix4x4<float> mvMatrix = context.modelViewMatrix * m_absoluteMatrix;
				mvMatrix *= Matrix4x4<float>::BuildFromScale(m_extent);
				glLoadMatrixf(&mvMatrix[0]);
				glColor4fv(&m_backgroundColor[0]);
			}
			RenderSquare(context);
		}
	}
}
