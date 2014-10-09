//- Centhra Engine -
#include <CE/AppFrontend.h>
#include <CE/Base.h>
#include <CE/Canvas.h>
#include <CE/UI/ButtonCtrl.h>
#include <CE/UI/ColorCtrl.h>

//- Standard Library -
#include <stdlib.h>

using namespace ce;
using namespace std;

//- Define your own implementation of the AppFrontend class. -
class AppTest : public AppFrontend
{
	Canvas *m_canvas;
	unsigned long long m_lastProcess;
	ui::Control *m_masterCtrl;

public:
	AppTest()
	{
		m_canvas = 0;
		m_lastProcess = 0;
		m_masterCtrl = 0;
	}

	//- Define the virtual functions for the class. -
	bool OnStart()
	{
		srand((unsigned int)GetRunTimeMS());
		m_canvas = Canvas::Create(640, 480, "000 - Test");

		m_masterCtrl = new ui::Control(Vector2<int_canvas>(0, 0), Vector2<int_canvas>(640, 480));

		ui::ColorCtrl *color1 = new ui::ColorCtrl(Vector2<int_canvas>(16, 16), Vector2<int_canvas>(64, 64), Color(255, 0, 0, 255));
		ui::ColorCtrl *color2 = new ui::ColorCtrl(Vector2<int_canvas>(32, 32), Vector2<int_canvas>(64, 64), Color(0, 255, 0, 255));
		ui::ColorCtrl *color3 = new ui::ColorCtrl(Vector2<int_canvas>(48, 48), Vector2<int_canvas>(64, 64), Color(0, 0, 255, 255));
		m_masterCtrl->Add(color1);
		m_masterCtrl->Add(color2);
		m_masterCtrl->Add(color3);

		ui::ButtonCtrl *button1 = new ui::ButtonCtrl(Vector2<int_canvas>(32, 128), Vector2<int_canvas>(64, 64));
		m_masterCtrl->Add(button1);

		return true;
	}
	bool OnProcess()
	{
		unsigned long long t = GetRunTimeMS();
		if((t - m_lastProcess) > 15) // 60-64 fps
		{
			float dt = (float)(t - m_lastProcess) / 1000.f;
			m_lastProcess = t;
		}

		sleepMS(1);
		return true;
	}
	void OnStopped()
	{
		delete m_canvas;
	}
	bool OnEvent(Event &event)
	{
		ui::Control *click = 0;
		switch(event.type)
		{
		case event::MouseButtonDown:
			click = m_masterCtrl->GetFromPosition(Vector2<int_canvas>(event.mouseButton.x, 480 - event.mouseButton.y));
			print("%d\t%d\t%d\n", event.mouseButton.x, 480 - event.mouseButton.y, click);
			click->OnEvent(event);
			break;
		case event::PostRender:
			m_masterCtrl->Render();
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
