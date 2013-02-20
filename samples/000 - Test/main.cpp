//- Centhra Engine -
#include <CE/AppFrontend.h>
#include <CE/Base.h>
#include <CE/Canvas.h>
#include <CE/Image.h>
#include <CE/Font.h>

//- OpenGL -
#include <GL/gl.h>
#include <GL/glu.h>

using namespace ce;

//- Define your own implementation of the AppFrontend class. -
class AppTest : public AppFrontend
{
	Canvas *m_canvas;
	Image *m_image;
	Font *m_font;

public:
	AppTest()
	{
		m_canvas = 0;
		m_image = 0;
	}

	//- Define the virtual functions for the class. -
	bool OnStart()
	{
		print("Initializing Image Library\n");
		Image::Init();
		Font::Init();

		m_canvas = Canvas::Create(300, 300, "201 - Image Rendering");

		print("Loading <../201 - Image Rendering/centhra.png>\n");
		m_image = Image::CreateFromFile("../201 - Image Rendering/centhra.png");
		if(m_image)
		{
			Vector2<int> imageSize = m_image->GetSize();
			print("  Width: %d Height: %d\n", imageSize.GetX(), imageSize.GetY());
		}
		else
			print("  Unable to load image.\n");

		print("Loading <../000 - Test/FreeMono.ttf>\n");
		m_font = Font::CreateFromFile("../000 - Test/FreeMono.ttf");
		if(m_font)
		{
			m_font->SetCharSize(0, 14*64, 96, 96);
		}
		else
			print("  Unable to load font.\n");

		glViewport(0, 0, 300, 300);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.f, 300.f, 0.f, 300.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT);

		return true;
	}
	bool OnProcess()
	{
		sleepMS(1);
		return true;
	}
	bool OnStop(bool force)
	{
		delete m_canvas;
		delete m_image;
		delete m_font;

		return true;
	}
	bool OnEvent(Event &event)
	{
		switch(event.type)
		{
			case event::PostRender:
				glClear(GL_COLOR_BUFFER_BIT);
				glClearColor(0.f, 0.f, 0.f, 1.f);
				glPushMatrix();
					glTranslatef(0.f, 100.f, 0.f);
					glColor4f(1.f, 0.f, 0.f, 1.f);
					glPushMatrix();
						m_font->DrawText("glPopMatrix();");
					glPopMatrix();
					glTranslatef(0.f, 20.f, 0.f);
					glColor4f(1.f, 1.f, 1.f, 1.f);
					glPushMatrix();
						m_font->DrawText("glPopMatrix();");
					glPopMatrix();
				glPopMatrix();
				break;
		}
		return true;
	}
};

int main(int argc, char **argv)
{
	print("201 - Image Rendering | Centhra Engine v%s\n", getVersionString().c_str());

	AppTest myApp;
	myApp.Start();

	//- Run the App's main loop. -
	while(myApp.IsRunning())
		myApp.Process();

	return 0;
}
