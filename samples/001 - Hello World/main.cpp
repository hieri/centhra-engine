//- Centhra Engine -
#include <CE/App.h>
#include <CE/Base.h>

using namespace ce;

//- Define your own implementation of the App class. -
class AppHelloWorld : public App
{
	int m_counter;

public:
	AppHelloWorld()
	{
		m_counter = 0;
	}

	//- Define the virtual functions for the class. -
	bool OnStart()
	{
		print("o-> OnStart\n");

		return true;
	}
	bool OnProcess()
	{
		m_counter++;

		print("| o-> OnProcess [%i/10]\n", m_counter);

		if(m_counter >= 10)
			return false;
		else
			sleepMS(1000);

		return true;
	}
	bool OnStop(bool force)
	{
		print("o-> OnStop\n");

		return true;
	}
};

int main(int argc, char **argv)
{
	print("001 - Hello World Sample | Centhra Engine v%s\n", getVersionString().c_str());

	AppHelloWorld myApp;
	myApp.Start();

	//- Run the App's main loop. -
	while(myApp.IsRunning())
		myApp.Process();

	return 0;
}
