//- Centhra Engine -
#include <CE/App.h>
#include <CE/Base.h>
#include <CE/Audio.h>

using namespace ce;

//- Define your own implementation of the App class. -
class AppSound : public App
{
	Audio *m_sample;

public:
	AppSound()
	{
		m_sample = 0;
	}

	//- Define the virtual functions for the class. -
	bool OnStart()
	{
		Audio::Init();

		m_sample = Audio::CreateFromFile("../101 - Sound/res/sample.wav");

		return true;
	}
	bool OnProcess()
	{
		print("Playing sample...\n");
		m_sample->Play2D();
		print("  Stopped.\n");
		Stop();

		return true;
	}
	void OnStopped()
	{
		Audio::Cleanup();
	}
};

int main(int argc, char **argv)
{
	print("101 - Sound Sample | Centhra Engine v%s\n", getVersionString().c_str());

	AppSound myApp;
	myApp.Start();

	//- Run the App's main loop. -
	while(myApp.IsRunning())
		myApp.Process();

	return 0;
}
