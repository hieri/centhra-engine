//- Centhra Engine -
#include <CE/AppFrontend.h>
#include <CE/Base.h>
#include <CE/Canvas.h>
#include <CE/Thread.h>
#include <CE/Mutex.h>
#include <CE/Socket.h>
#include <CE/Game2D/Camera.h>
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Game2D/PhysicalGroup.h>
#include <CE/Game2D/DefaultPhysicsHandler.h>
#include <CE/UI/CameraView2DCtrl.h>

//- Standard Library -
#include <cstdlib>
#include <string>
#include <ctime>

using namespace ce;
using namespace std;

#define NUMRANDOMS 256

void *physicsFunc(void *arg);
void *connectionFunc(void *arg);
Mutex g_physicsMutex;

//- Define your own implementation of the AppFrontend class. -
class AppTest : public AppFrontend
{
	Canvas *m_canvas;
	game2d::PhysicalGroup *m_group;
	game2d::PhysicalObject *m_entity;
	game2d::Camera *m_camera;
	ui::CameraView2DCtrl *m_view;
	game2d::DefaultPhysicsHandler *m_defaultPhysicsHandler;
	bool w,a,s,d;
	unsigned long m_lastProcess;

public:
	Thread *m_physicsThread, *m_connectionThread;

	AppTest()
	{
		m_canvas = 0;
		m_defaultPhysicsHandler = 0;
		m_entity = 0;
		m_camera = 0;
		m_group = 0;
		m_view = 0;
		m_lastProcess = 0;
		w = a = s = d = false;
		m_physicsThread = new Thread(&physicsFunc);
		m_connectionThread = new Thread(&connectionFunc);
	}
	~AppTest()
	{
		m_physicsThread->Join();
		delete m_physicsThread;

		m_connectionThread->Join();
		delete m_connectionThread;
	}

	//- Define the virtual functions for the class. -
	bool OnStart()
	{
		m_canvas = Canvas::Create(640, 480, "550 - 2D Client");

		m_group = new game2d::PhysicalGroup();
		m_entity = new game2d::PhysicalObject(Vector2<float>(512.f, 512.f), Vector2<float>(32.f, 32.f));
		m_group->Add(m_entity);

		m_camera = new game2d::Camera();
		m_camera->SetFocus(m_entity);

		m_view = new ui::CameraView2DCtrl(Vector2<int>(0, 0), Vector2<int>(640, 480));
		m_view->SetCamera(m_camera);

		m_defaultPhysicsHandler = new game2d::DefaultPhysicsHandler();
		m_group->AttachHandler(m_defaultPhysicsHandler);

		g_physicsMutex.Init();
		m_physicsThread->Start(this);
		m_connectionThread->Start(this);
		return true;
	}
	void ProcessPhysics(float dt)
	{
		Vector2<float> dif;
		if(w)
			dif[1] += 1.f;
		if(a)
			dif[0] -= 1.f;
		if(s)
			dif[1] -= 1.f;
		if(d)
			dif[0] += 1.f;
		dif *= 256.f;
		m_entity->SetVelocity(dif);

		m_group->ProcessPhysics(dt);
//		m_plane->RemoveDead();
//		game2d::Entity::DeleteDead();
	}
	bool OnProcess()
	{
		unsigned long t = GetRunTimeMS();
		if((t - m_lastProcess) > 15)
		{
			float dt = (float)(t - m_lastProcess) / 1000.f;
			m_lastProcess = t;

		//	ProcessPhysics(dt);
		}

		sleepMS(1);
		return true;
	}
	void OnStopped()
	{
		m_group->DetachHandler();
		delete m_defaultPhysicsHandler;

		g_physicsMutex.Destroy();

		delete m_view;
		delete m_camera;
		game2d::Entity::DeleteDead();
		delete m_entity;
		delete m_group;
//		game2d::ZoneEntity::Cleanup();
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
			g_physicsMutex.Lock();
			m_view->Render();
			g_physicsMutex.Unlock();
			break;
		case event::WindowResize:
			if(m_view)
				m_view->SetExtent(Vector2<int>(event.windowResize.width, event.windowResize.height));
			break;
		}
		return true;
	}
};

void *connectionFunc(void *arg)
{
	AppTest *app = (AppTest *)arg;
	unsigned long lastProcess = app->GetRunTimeMS();

	Socket *client = Socket::Create(Socket::IP4, Socket::Stream, Socket::TCP);
	string connectionMsg = "2dClient";

	bool isConnected = false;


	while(app->IsRunning())
	{
		unsigned long t = app->GetRunTimeMS();
		if((t - lastProcess) > 32)
		{
//			float dt = (float)(t - lastProcess) / 1000.f;
			lastProcess = t;

			if(isConnected)
			{
			}
			else
			{
				if(client->Connect("127.0.0.1", 12346))
				{
					isConnected = true;
					client->Write((char *)connectionMsg.c_str(), connectionMsg.length());
				}
				else
					app->Stop(); //- TODO: Handle this better -
			}
		}

		sleepMS(1);
	}

	delete client;

	Thread::Exit(NULL);
	return 0;
}

void *physicsFunc(void *arg)
{
	AppTest *app = (AppTest *)arg;
	unsigned long lastProcess = app->GetRunTimeMS();

	while(app->IsRunning())
	{
		unsigned long t = app->GetRunTimeMS();
		if((t - lastProcess) > 15)
		{
			float dt = (float)(t - lastProcess) / 1000.f;
			lastProcess = t;

			g_physicsMutex.Lock();
			app->ProcessPhysics(dt);
			g_physicsMutex.Unlock();
		}

		sleepMS(1);
	}

	Thread::Exit(NULL);
	return 0;
}

int main(int argc, char **argv)
{
	print("550 - 2D Client | Centhra Engine v%s\n", getVersionString().c_str());

	AppTest myApp;
	myApp.Start();

	//- Run the App's main loop. -
	while(myApp.IsRunning())
		myApp.Process();

	Thread::Exit(NULL);
	return 0;
}
