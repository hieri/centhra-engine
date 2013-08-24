//- Centhra Engine -
#include <CE/AppFrontend.h>
#include <CE/Base.h>
#include <CE/Canvas.h>
#include <CE/Thread.h>
#include <CE/Game2D/Camera.h>
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Game2D/PhysicalGroup.h>
#include <CE/Game2D/DefaultPhysicsHandler.h>
#include <CE/UI/CameraView2DCtrl.h>

//- Standard Library -
#include <stdlib.h>
#include <time.h>

using namespace ce;

#define NUMRANDOMS 1024

void *physicsFunc(void *arg);

//- Define your own implementation of the AppFrontend class. -
class AppTest : public AppFrontend
{
	Canvas *m_canvas;
	game2d::PhysicalGroup *m_group;
	game2d::PhysicalObject *m_entity, **m_randoms;
	game2d::Camera *m_camera;
	ui::CameraView2DCtrl *m_view;
	game2d::DefaultPhysicsHandler *m_defaultPhysicsHandler;
	bool w,a,s,d;
	unsigned long m_lastProcess;

public:
//	Thread* m_physicsThread;

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
		m_randoms = 0;
//		m_physicsThread = new Thread(&physicsFunc);
	}
	~AppTest()
	{
//		m_physicsThread->Join();
//		delete m_physicsThread;
	}

	//- Define the virtual functions for the class. -
	bool OnStart()
	{
		srand((unsigned int)time(NULL));
		m_canvas = Canvas::Create(640, 480, "500 - 2D Collision");
		m_group = new game2d::PhysicalGroup();
		m_entity = new game2d::PhysicalObject(Vector2<float>(512.f, 512.f), Vector2<float>(32.f, 32.f));
		m_group->Add(m_entity);

		m_camera = new game2d::Camera();
		m_camera->SetFocus(m_entity);

		m_view = new ui::CameraView2DCtrl(Vector2<int>(0, 0), Vector2<int>(640, 480));
		m_view->SetCamera(m_camera);

		m_entity->SetCollisionMask(0);

		m_randoms = new game2d::PhysicalObject *[NUMRANDOMS];
		bool randBuff[4096];
		for(unsigned int a = 0; a < 4096; a++)
			randBuff[a] = true;
		randBuff[32 * 64 + 32] = false;
		for(unsigned int a = 0; a < NUMRANDOMS; a++)
		{
			unsigned int rx, ry;
			do
			{
				rx = rand() % 64;
				ry = rand() % 64;
			}
			while(!randBuff[ry * 64 + rx]);
			randBuff[ry * 64 + rx] = false;

			m_randoms[a] = new game2d::PhysicalObject(Vector2<float>((float)rx * 16.f, (float)ry * 16.f), Vector2<float>(16.f, 16.f));

			Vector2<float> dif = Vector2<float>((float)(rand() % 1024 - 512), (float)(rand() % 1024 - 512));
			m_randoms[a]->SetVelocity(dif);
			m_group->Add(m_randoms[a]);
		}

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
			dif *= 256.f;
			m_entity->SetVelocity(dif);

			
			Vector2<float> origin = Vector2<float>(m_entity->GetPosition()[0] + 16.f, m_entity->GetPosition()[1] + 16.f);
//				origin = Vector2<float>(512.f, 512.f);

			for(int a = 0; a < NUMRANDOMS; a++)
			{
				Vector2<float> pos = m_randoms[a]->GetPosition();
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
				m_randoms[a]->SetVelocity(vel);
			}

			m_group->ProcessPhysics(dt);
//			m_plane->RemoveDead();
//			game2d::Entity::DeleteDead();
		}

		sleepMS(1);
		return true;
	}
	void OnStopped()
	{
		m_group->DetachHandler();
		delete m_defaultPhysicsHandler;

		delete m_view;
		delete m_camera;
		game2d::Entity::DeleteDead();
		delete m_entity;
		for(int a = 0; a < NUMRANDOMS; a++)
			delete m_randoms[a];
		delete [] m_randoms;
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
			m_view->Render();
			break;
		case event::WindowResize:
			if(m_view)
				m_view->SetExtent(Vector2<int>(event.windowResize.width, event.windowResize.height));
			break;
		}
		return true;
	}
};

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

//			app->m_plane->ProcessPhysics(dt);
		}

		sleepMS(1);
	}

	Thread::Exit(NULL);
	return 0;
}

int main(int argc, char **argv)
{
	print("500 - 2D Collision | Centhra Engine v%s\n", getVersionString().c_str());

	AppTest myApp;
	myApp.Start();

	//- Run the App's main loop. -
	while(myApp.IsRunning())
		myApp.Process();

//	Thread::Exit(NULL);
	return 0;
}
