//- Standard Library -
#include <cstdlib>
#include <ctime>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Thread.h>
#include <CE/Mutex.h>
#include <CE/Game2D.h>
#include <CE/Plugin/Box2D/PhysicsHandler.h>
#include <CE/Renderer.h>

using namespace ce;

#define NUMRANDOMS 256

void *physicsFunc(void *arg);
Mutex physicsMutex;

//#define RENDER_PROFILE

class AppBox2DSample : public AppGame2D
{
	Canvas *m_canvas;
	game2d::PhysicalGroup *m_group;
	game2d::PhysicalObject *m_entity, **m_randoms;
	game2d::Camera *m_camera;
	ui::CameraView2DCtrl *m_view;
	plugin::box2d::bPhysicsHandler *m_box2dPhysicsHandler;
	bool w,a,s,d;
	unsigned long long m_lastProcess;

public:
	Thread* m_physicsThread;

	AppBox2DSample() : AppGame2D()
	{
		m_canvas = 0;
		m_box2dPhysicsHandler = 0;
		m_entity = 0;
		m_camera = 0;
		m_group = 0;
		m_view = 0;
		m_lastProcess = 0;
		w = a = s = d = false;
		m_randoms = 0;
		m_physicsThread = new Thread(&physicsFunc);
	}
	~AppBox2DSample()
	{
		delete m_physicsThread;
	}
	bool OnStart()
	{
		bool result = AppGame2D::OnStart();
		if(!result)
			return false;

		setVerboseLogging(true);
		srand((unsigned int)time(NULL));

		m_canvas = Canvas::Create(640, 480, "901 - Box2D");
		#ifdef RENDER_PROFILE
			m_canvas->SetVSync(false);
		#endif

		m_group = new game2d::PhysicalGroup();
		m_entity = new game2d::PhysicalObject(Vector2<float>(512.f, 512.f), Vector2<float>(32.f, 32.f));
		m_group->Add(m_entity);

		m_camera = new game2d::Camera(m_group);
		m_camera->SetFocus(m_entity);

		m_view = new ui::CameraView2DCtrl(Vector2<int_canvas>(0, 0), Vector2<int_canvas>(640, 480));
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

		m_box2dPhysicsHandler = new plugin::box2d::bPhysicsHandler();
		m_group->AttachHandler(m_box2dPhysicsHandler);

		physicsMutex.Init();
		m_physicsThread->Start(this);
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
		
		Vector2<float> origin = Vector2<float>(m_entity->GetPosition()[0], m_entity->GetPosition()[1]);
//		origin = Vector2<float>(512.f, 512.f);

		for(int a = 0; a < NUMRANDOMS; a++)
		{
			Vector2<float> pos = m_randoms[a]->GetPosition();
//			Vector2<float> vel = Vector2<float>((float)(rand() % 512 - 256), (float)(rand() % 512 - 256));
//			Vector2<float> vel = m_randoms[a]->GetVelocity();
//			Vector2<float> vel = Vector2<float>(origin[0] - pos[0], origin[1] - pos[1]);
			Vector2<float> vel = Vector2<float>(origin[1] - pos[1], pos[0] - origin[0]);
//			vel /= vel.GetLength();
//			vel *= 64.f;

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
	}
	bool OnProcess()
	{
		unsigned long long t = GetRunTimeMS();
		if((t - m_lastProcess) > 15)
		{
			float dt = (float)(t - m_lastProcess) / 1000.f;
			m_lastProcess = t;

		//	ProcessPhysics(dt);
		}
		
		#ifndef RENDER_PROFILE
			sleepMS(1);
		#endif
		return true;
	}
	void OnStopped()
	{
		m_physicsThread->Join();
		physicsMutex.Destroy();

		m_group->DetachHandler();
		delete m_box2dPhysicsHandler;

		delete m_view;
		delete m_camera;
		game2d::Entity::FinalizeDelete();
		delete m_entity;
		for(int a = 0; a < NUMRANDOMS; a++)
			delete m_randoms[a];
		delete [] m_randoms;
		delete m_group;

		delete m_canvas;

		AppGame2D::OnStopped();
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
			physicsMutex.Lock();
			m_view->Render(m_canvas);
			physicsMutex.Unlock();
			break;
		case event::WindowResize:
			if(m_view)
				m_view->SetExtent(Vector2<int_canvas>(event.windowResize.width, event.windowResize.height));
			break;
		}
		return true;
	}
};

void *physicsFunc(void *arg)
{
	AppBox2DSample *app = (AppBox2DSample *)arg;
	unsigned long long lastProcess = app->GetRunTimeMS();

	while(app->IsRunning())
	{
		unsigned long long t = app->GetRunTimeMS();
		if((t - lastProcess) > 15)
		{
			float dt = (float)(t - lastProcess) / 1000.f;
			lastProcess = t;

			physicsMutex.Lock();
			app->ProcessPhysics(dt);
			physicsMutex.Unlock();
		}

		sleepMS(1);
	}

	Thread::Exit(NULL);
	return 0;
}

int main(int argc, char **argv)
{
	print("901 - Box2D | Centhra Engine v%s\n", getVersionString().c_str());

	AppBox2DSample myApp;
	myApp.Start();

	while(myApp.IsRunning())
		myApp.Process();

	return 0;
}
