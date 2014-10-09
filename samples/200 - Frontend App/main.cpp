//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Frontend.h>

using namespace ce;

//- Define your own implementation of the AppFrontend class. -
class AppFrontendSample : public AppFrontend
{
	Canvas *m_canvas;

public:
	AppFrontendSample()
	{
		m_canvas = 0;
	}
	~AppFrontendSample()
	{
	}

	//- Define the virtual functions for the class. -
	bool OnStart()
	{
		print("o-> OnStart\n");
		m_canvas = Canvas::Create(640, 480, "200 - Frontend App Sample");

		return true;
	}
	bool OnProcess()
	{
		sleepMS(1);
		return true;
	}
	void OnStopped()
	{
		print("o-> OnStopped\n");
		delete m_canvas;
	}
	bool OnEvent(Event &event)
	{
		switch(event.type)
		{
		case event::KeyDown:
			print("Key Down: %d\n", event.key.keyCode);
			break;
		case event::KeyUp:
			print("Key Up: %d\n", event.key.keyCode);
			break;
		case event::WindowResize:
			print("Window Resize: %d %d\n", event.windowResize.width, event.windowResize.height);
			break;
		}
		return true;
	}
};

int main(int argc, char **argv)
{
	print("200 - Frontend App Sample | Centhra Engine v%s\n", getVersionString().c_str());

	AppFrontendSample myApp;

	myApp.Start();

	//- Run the App's main loop. -
	while(myApp.IsRunning())
		myApp.Process();

	return 0;
}
