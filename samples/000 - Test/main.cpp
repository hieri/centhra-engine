//- Centhra Engine -
#include <CE/AppFrontend.h>
#include <CE/Base.h>
#include <CE/Canvas.h>

#include <CE/UI/CameraView2DCtrl.h>
#include <CE/Game2D/Camera.h>
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Game2D/PhysicalGroup.h>

using namespace ce;
using namespace std;

//- Define your own implementation of the AppFrontend class. -
class AppTest : public AppFrontend
{
	Canvas *m_canvas;
	ui::CameraView2DCtrl *m_cameraCtrl;
	game2d::Camera *m_camera;
	game2d::PhysicalGroup *m_group;
	game2d::PhysicalObject *m_objects[16];
	game2d::PhysicalObject *m_player;
	unsigned long m_lastProcess;

public:
	AppTest()
	{
		m_canvas = 0;
		m_lastProcess = 0;
	}

	//- Define the virtual functions for the class. -
	bool OnStart()
	{
		m_canvas = Canvas::Create(800, 600, "000 - Test");
		m_cameraCtrl = new ui::CameraView2DCtrl(Vector2<int>(0, 0), Vector2<int>(800, 600));
 
		m_group = new game2d::PhysicalGroup();

		for(int a = 0; a < 16; a++)
		{
			m_objects[a] = new game2d::PhysicalObject(Vector2<float>((float)(rand() % 1024 - 512), (float)(rand() % 1024 - 512)), Vector2<float>(16.f, 16.f));
			m_group->Add(m_objects[a]);
		}

		m_player = new game2d::PhysicalObject(Vector2<float>(10.f, 0.f), Vector2<float>(32.f, 32.f));
		m_group->Add(m_player);

		m_camera = new game2d::Camera();
		m_camera->SetFocus(m_player);
		m_cameraCtrl->SetCamera(m_camera);

		return true;
	}
	bool OnProcess()
	{
		unsigned long t = GetRunTimeMS();
		if((t - m_lastProcess) > 15)
		{
			float dt = (float)(t - m_lastProcess) / 1000.f;
			m_lastProcess = t;

			for(int a = 0; a < 16; a++)
				m_objects[a]->SetVelocity(Vector2<float>((float)(rand() % 1024 - 512), (float)(rand() % 1024 - 512)));
			

			vector<Group::Member *> &members = m_group->GetMembers();
			for(vector<Group::Member *>::iterator it = members.begin(); it != members.end(); it++)
			{
				game2d::PhysicalObject *object = (game2d::PhysicalObject *)*it;
				object->SetPosition(object->GetPosition() + object->GetVelocity() * dt);
			}
		}

		sleepMS(1);
		return true;
	}
	void OnStopped()
	{
		for(int a = 0; a < 16; a++)
			delete m_objects[a];
		delete m_player;
		delete m_cameraCtrl;
		delete m_camera;
		delete m_group;
		delete m_canvas;
	}
	bool OnEvent(Event &event)
	{
		if(event.type == event::PostRender)
			m_cameraCtrl->Render();

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
