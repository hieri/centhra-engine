//- Centhra Engine -
#include <CE/AppFrontend.h>
#include <CE/Base.h>
#include <CE/Canvas.h>

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
	bool OnStop(bool force)
	{
		print("o-> OnStop\n");
		delete m_canvas;

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
