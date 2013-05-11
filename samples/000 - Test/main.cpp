//- Centhra Engine -
#include <CE/AppFrontend.h>
#include <CE/Base.h>
#include <CE/Canvas.h>
#include <CE/Font.h>
#include <CE/UI/TextEditCtrl.h>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

using namespace ce;

//- Define your own implementation of the AppFrontend class. -
class AppTest : public AppFrontend
{
	Canvas *m_canvas;
	Font *m_font;
	ui::TextEditCtrl *m_textEditCtrl;

public:
	AppTest()
	{
		m_canvas = 0;
		m_textEditCtrl = 0;
	}

	//- Define the virtual functions for the class. -
	bool OnStart()
	{
		print("Initializing Font Library\n");
		Font::Init();

		m_font = Font::CreateFromFile("../000 - Test/FreeMono.ttf");
		m_font->SetCharSize(0, 12 * 64, 96, 96);
		m_textEditCtrl = new ui::TextEditCtrl(Vector2<int>(0, 0), Vector2<int>(800, 600), m_font);
		m_textEditCtrl->SetText("adsfaf");
 
		m_canvas = Canvas::Create(800, 600, "000 - Test");
		return true;
	}
	bool OnProcess()
	{
		sleepMS(1);
		return true;
	}
	void OnStopped()
	{
		delete m_textEditCtrl;
		delete m_font;
		delete m_canvas;
	}
	bool OnEvent(Event &event)
	{
		switch(event.type)
		{
		case event::KeyDown:
		case event::KeyUp:
			m_textEditCtrl->OnEvent(event);
			break;
		case event::PostRender:
			m_textEditCtrl->Render();
			break;
		}
		return true;
	}
};

int main(int argc, char **argv)
{
	print("000 - Test | Centhra Engine v%s\n", getVersionString().c_str());

	AppTest myApp;
	myApp.Start();

	//- Run the App's main loop. -
	while(myApp.IsRunning())
		myApp.Process();

	return 0;
}
