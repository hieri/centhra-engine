//- Centhra Engine -
#include <CE/AppFrontend.h>
#include <CE/Base.h>
#include <CE/Canvas.h>
#include <CE/UI/CameraView2DCtrl.h>
#include <CE/Game2D/Camera.h>
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Game2D/PhysicalGroup.h>
#include <CE/Game2D/DefaultPhysicsHandler.h>

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
	game2d::DefaultPhysicsHandler *m_defaultPhysicsHandler;
	unsigned long m_lastProcess;
	bool w,a,s,d;

public:
	AppTest()
	{
		m_canvas = 0;
		m_lastProcess = 0;
		w = a = s = d = false;
	}

	//- Define the virtual functions for the class. -
	bool OnStart()
	{
		m_canvas = Canvas::Create(800, 600, "000 - Test");
		m_cameraCtrl = new ui::CameraView2DCtrl(Vector2<int>(0, 0), Vector2<int>(800, 600));
 
		m_group = new game2d::PhysicalGroup();

		for(int a = 0; a < 16; a++)
		{
			m_objects[a] = new game2d::PhysicalObject(Vector2<float>((float)(rand() % 1024), (float)(rand() % 1024)), Vector2<float>(16.f, 16.f));
			m_group->Add(m_objects[a]);
		}

		m_player = new game2d::PhysicalObject(Vector2<float>(512.f, 512.f), Vector2<float>(48.f, 48.f));
		m_group->Add(m_player);

		m_camera = new game2d::Camera();
		m_camera->SetFocus(m_player);
		m_cameraCtrl->SetCamera(m_camera);

		m_defaultPhysicsHandler = new game2d::DefaultPhysicsHandler();
		m_group->AttachHandler(m_defaultPhysicsHandler);

		return true;
	}
	bool OnProcess()
	{
		unsigned long t = GetRunTimeMS();
		if((t - m_lastProcess) > 15)
		{
			float dt = (float)(t - m_lastProcess) / 1000.f;
			m_lastProcess = t;

			Vector2<float> dif;
			if(w)
				dif[1] += 1.f;
			if(a)
				dif[0] -= 1.f;
			if(s)
				dif[1] -= 1.f;
			if(d)
				dif[0] += 1.f;
			if(dif.GetLength() != 0.f)
				dif /= dif.GetLength();
			dif *= 256.f;
			m_player->SetVelocity(dif);

			m_group->ProcessPhysics(dt);

			Vector2<float> origin = Vector2<float>(m_player->GetPosition()[0] + 16.f, m_player->GetPosition()[1] + 16.f);
//				origin = Vector2<float>(512.f, 512.f);

			for(int a = 0; a < 16; a++)
			{
				Vector2<float> pos = m_objects[a]->GetPosition();
//				Vector2<float> vel = Vector2<float>((float)(rand() % 512 - 256), (float)(rand() % 512 - 256));
//				Vector2<float> vel = m_randoms[a]->GetVelocity();
//				Vector2<float> vel = Vector2<float>(origin[0] - pos[0], origin[1] - pos[1]);
				Vector2<float> vel = Vector2<float>(origin[1] - pos[1], pos[0] - origin[0]);
//				vel /= vel.GetLength();
//				vel *= 64.f;

				if(pos[0] > 1008.f && vel[0] > 0)
					vel[0] *= -1.f;
				if(pos[0] < 0.f && vel[0] < 0)
					vel[0] *= -1.f;
				if(pos[1] > 1008.f && vel[1] > 0)
					vel[1] *= -1.f;
				if(pos[1] < 0.f && vel[1] < 0)
					vel[1] *= -1.f;
				m_objects[a]->SetVelocity(vel);
			}
/*			for(int a = 0; a < 16; a++)
				m_objects[a]->SetVelocity(Vector2<float>((float)(rand() % 1024 - 512), (float)(rand() % 1024 - 512)));
			

			vector<Group::Member *> &members = m_group->GetMembers();
			for(vector<Group::Member *>::iterator it = members.begin(); it != members.end(); it++)
			{
				game2d::PhysicalObject *object = (game2d::PhysicalObject *)*it;
				object->SetPosition(object->GetPosition() + object->GetVelocity() * dt);
			}
*/
		}

		sleepMS(1);
		return true;
	}
	void OnStopped()
	{
		m_group->CleanupHandler();
		delete m_defaultPhysicsHandler;

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
		switch(event.type)
		{
		case event::KeyDown:
			switch(event.key.keyCode)
			{
			case Key_W:
				w = true;
				break;
			case Key_A:
				a = true;
				break;
			case Key_S:
				s = true;
				break;
			case Key_D:
				d = true;
				break;
			}
			break;
		case event::KeyUp:
			switch(event.key.keyCode)
			{
			case Key_W:
				w = false;
				break;
			case Key_A:
				a = false;
				break;
			case Key_S:
				s = false;
				break;
			case Key_D:
				d = false;
				break;
			}
			break;
		case event::PostRender:
			m_cameraCtrl->Render();
			break;
		case event::WindowResize:
			if(m_cameraCtrl)
				m_cameraCtrl->SetExtent(Vector2<int>(event.windowResize.width, event.windowResize.height));
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
