//- Centhra Engine -
#include <CE/AppFrontend.h>
#include <CE/Base.h>
#include <CE/Canvas.h>
#include <CE/Font.h>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>
#include <GL/glu.h>

using namespace ce;

//- Define your own implementation of the AppFrontend class. -
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

	//- Define the virtual functions for the class. -
	bool OnStart()
	{
		print("Initializing Font Library\n");
		Font::Init();

		m_canvas = Canvas::Create(300, 300, "202 - Font Rendering");

		print("Loading <../202 - Font Rendering/res/FreeMono.ttf>\n");
		m_font = Font::CreateFromFile("../202 - Font Rendering/res/FreeMono.ttf");
		if(m_font)
			m_font->SetCharSize(0, 14*64, 96, 96);
		else
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
				glPushMatrix();
					glTranslatef(0.f, 8.f, 0.f);
					glColor4f(1.f, 0.f, 0.f, 1.f);
					glPushMatrix();
						m_font->DrawString("Hello font rendering!");
					glPopMatrix();
					glTranslatef(0.f, 20.f, 0.f);
					glColor4f(1.f, 1.f, 1.f, 1.f);
					glPushMatrix();
						m_font->DrawString("Hello font rendering!");
					glPopMatrix();
					glTranslatef(0.f, 20.f, 0.f);
					glColor4f(0.f, 0.f, 1.f, 1.f);
					glPushMatrix();
						m_font->DrawString("Hello font rendering!");
					glPopMatrix();
					glTranslatef(0.f, 20.f, 0.f);
					glColor4f(0.f, 1.f, 0.f, 1.f);
					glPushMatrix();
						m_font->DrawString("Hello font rendering!");
					glPopMatrix();
				glPopMatrix();
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

	//- Run the App's main loop. -
	while(myApp.IsRunning())
		myApp.Process();

	return 0;
}
