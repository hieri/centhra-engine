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

public:
	AppUserInterfaceSample()
	{
		m_canvas = 0;
	}
	bool OnStart()
	{
		m_canvas = Canvas::Create(1280, 720, "300 - User Interface");

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
	}
	bool OnEvent(Event &event)
	{
		switch(event.type)
		{
			case event::PostRender:
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
