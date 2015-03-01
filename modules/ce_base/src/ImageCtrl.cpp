#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/UI/ImageCtrl.h>
#include <CE/Renderer.h>
#include <CE/Base.h>

using namespace std;

namespace ce
{
	namespace ui
	{
		ImageCtrl::ImageCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Image *image, Color<float> color) : ColorCtrl(position, extent, color)
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
			ConfigTextured();
			ShaderProgram::TexturedProgram *program = 0;
			if(context.useShaders)
				program = UseTexturedProgram();
			if(program != 0)
			{
				Matrix4x4<float> mvpMatrix = context.projectionMatrix * m_absoluteMatrix;
				mvpMatrix *= Matrix4x4<float>::BuildFromScale(m_extent);
				glUniformMatrix4fv(program->mvpMatrix, 1, GL_FALSE, &mvpMatrix[0]);
			}
			else
			{
				Matrix4x4<float> mvMatrix = context.modelViewMatrix * m_absoluteMatrix;
				mvMatrix *= Matrix4x4<float>::BuildFromScale(m_extent);
				//TODO: Determine if this is necessary
				glColor4ub(255, 255, 255, 255);
				glLoadMatrixf(&mvMatrix[0]);
			}
			RenderSquareTexturedFull(context);
		}
	}
}
