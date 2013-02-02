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
	bool onStart()
	{
		print("o-> onStart\n");
		m_canvas = Canvas::create(640, 480, "200 - Frontend App Sample");

		return true;
	}
	bool onLoop()
	{
		sleep(1);
		return true;
	}
	bool onQuit(bool force)
	{
		print("o-> onQuit\n");
		delete m_canvas;

		return true;
	}
};

int main(int argc, char **argv)
{
	print("200 - Frontend App Sample | Centhra Engine v%s\n", getVersionString().c_str());

	AppFrontendSample myApp;
	myApp.start();

	//- Run the App's main loop. -
	while(myApp.isRunning())
		myApp.process();

	return 0;
}
