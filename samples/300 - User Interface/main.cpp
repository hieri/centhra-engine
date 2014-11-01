#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>
#include <GL/glu.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Font.h>
#include <CE/Frontend.h>
#include <CE/UI.h>

using namespace ce;

class AppUserInterfaceSample : public AppFrontend
{
	Canvas *m_canvas;
	ui::Control *m_rootCtrl;
	Font *m_font;

public:
	AppUserInterfaceSample()
	{
		m_canvas = 0;
		m_rootCtrl = 0;
		m_font = 0;
	}
	bool OnStart()
	{
		m_canvas = Canvas::Create(1280, 720, "300 - User Interface");

		Font::Init();
		m_font = Font::CreateFromFile("../res/FreeMono.ttf");
		if(m_font)
			m_font->SetCharSize(0, 14 * 64, 96, 96);

		m_rootCtrl = new ui::Control(Vector2<int_canvas>(0, 0), Vector2<int_canvas>(1280, 720));

		{
			m_rootCtrl->Add(new ui::ColorCtrl(Vector2<int_canvas>(0, 0), Vector2<int_canvas>(64, 64), Color(255, 0, 0)));
			m_rootCtrl->Add(new ui::ColorCtrl(Vector2<int_canvas>(64, 64), Vector2<int_canvas>(64, 64), Color(255, 255, 0)));
			m_rootCtrl->Add(new ui::TextCtrl(Vector2<int_canvas>(64, 0), Vector2<int_canvas>(256, 64), m_font, "Hello World!\nTEST\nOMG YEs", Color(255, 0, 0)));
		}

		return true;
	}
	bool OnProcess()
	{
		sleepMS(1);
		return true;
	}
	void OnStopped()
	{
		delete m_rootCtrl;

		delete m_font;
		delete m_canvas;
	}
	bool OnEvent(Event &event)
	{
		switch(event.type)
		{
			case event::PostRender:
				m_rootCtrl->Render(m_canvas);
				break;
		}
		return true;
	}
};

int main(int argc, char **argv)
{
	print("300 - User Interface | Centhra Engine v%s\n", getVersionString().c_str());

	AppUserInterfaceSample myApp;
	myApp.Start();

	while(myApp.IsRunning())
		myApp.Process();

	return 0;
}
