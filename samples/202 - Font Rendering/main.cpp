#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>
#include <GL/glu.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Image.h>
#include <CE/Font.h>
#include <CE/Frontend.h>
#include <CE/Renderer.h>

using namespace ce;

class AppFontRenderingSample : public AppFrontend
{
	Canvas *m_canvas;
	Font *m_font;

public:
	AppFontRenderingSample()
	{
		m_canvas = 0;
		m_font = 0;
	}
	bool OnStart()
	{
		m_canvas = Canvas::Create(300, 300, "202 - Font Rendering");

		print("Initializing Font Library\n");
		Image::Init();
		Font::Init();

		print("Loading <../res/FreeMono.ttf>\n");
		m_font = Font::CreateFromFile("../res/FreeMono.ttf", 14, m_canvas->GetHorizontalDPI(), m_canvas->GetVerticalDPI());

		if(m_font == 0)
			print("  Unable to load font.\n");

		return true;
	}
	bool OnProcess()
	{
		sleepMS(1);
		return true;
	}
	void OnStopped()
	{
		delete m_canvas;
		delete m_font;
	}
	bool OnEvent(Event &event)
	{
		switch(event.type)
		{
			case event::PostRender:
				{
			/*		RenderContext context;
					context.useShaders = false;
					glPushMatrix();
					glEnable(GL_TEXTURE_2D);
					glScalef(300.f, 300.f, 1.f);
					glBindTexture(GL_TEXTURE_2D, 1);
						glBegin(GL_QUADS);
							glTexCoord2i(0, 1);
							glVertex2i(0, 1);

							glTexCoord2i(0, 0);
							glVertex2i(0, 0);

							glTexCoord2i(1, 0);
							glVertex2i(1, 0);

							glTexCoord2i(1, 1);
							glVertex2i(1, 1);
						glEnd();
					glDisable(GL_TEXTURE_2D);
					glPopMatrix();
					*/
					glEnableClientState(GL_VERTEX_ARRAY);
					RenderContext context;
					context.modelViewMatrix = Matrix4x4<float>();
					context.projectionMatrix = m_canvas->GetProjectionMatrix();
					context.mvpMatrix = context.projectionMatrix * context.modelViewMatrix;
					context.useShaders = false;
					glPushMatrix();
					glScalef(1.f, 1.f, 1.f);
					m_font->DrawStringUI(context, "Hello World!\nHello New Line!!");
					glPopMatrix();
					glDisableClientState(GL_VERTEX_ARRAY);
				}
				break;
		}
		return true;
	}
};

int main(int argc, char **argv)
{
	print("202 - Font Rendering | Centhra Engine v%s\n", getVersionString().c_str());

	AppFontRenderingSample myApp;
	myApp.Start();

	while(myApp.IsRunning())
		myApp.Process();

	return 0;
}
