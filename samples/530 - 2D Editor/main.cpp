//- Standard Library -
#include <cstdlib>
#include <ctime>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Thread.h>
#include <CE/Frontend.h>
#include <CE/Game2D.h>
//#include <CE/Game2D/DefaultPhysicsHandler.h>
#include <CE/Plugin/Box2D/PhysicsHandler.h>
#include <CE/Game2D/AppGame2D.h>
#include <CE/UI/Editor2D.h>

using namespace ce;
using namespace std;

#define NUMRANDOMS 32

void *physicsFunc(void *arg);

class App2DEditorSample : public game2d::AppGame2D
{
	Canvas *m_canvas;
	game2d::PhysicalGroup *m_group;
	game2d::PhysicalObject *m_entity, **m_randoms;
	game2d::Camera *m_camera;
	ui::CameraView2DCtrl *m_view;
//	game2d::DefaultPhysicsHandler *m_defaultPhysicsHandler;
	plugin::box2d::bPhysicsHandler *m_box2dPhysicsHandler;
	ui::Editor2DCtrl *m_editorCtrl;
	Font *m_font;
	bool m_isEditMode;
	bool w,a,s,d;
	unsigned long long m_lastProcess;

public:
	Thread* m_physicsThread;

	App2DEditorSample() : AppGame2D()
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
		m_editorCtrl = 0;
		m_font = 0;
		m_isEditMode = false;
		m_physicsThread = new Thread(&physicsFunc);
	}
	~App2DEditorSample()
	{
		delete m_physicsThread;
	}
	bool OnStart()
	{
		srand((unsigned int)time(NULL));

		m_canvas = Canvas::Create(640, 480, "530 - 2D Editor");
		m_group = new game2d::PhysicalGroup();
		m_entity = new game2d::PhysicalObject(Vector2<float>(512.f, 512.f), Vector2<float>(32.f, 32.f));
		m_group->Add(m_entity);
		SetReferenceObject(m_entity);

		m_camera = new game2d::Camera();
		m_camera->SetFocus(m_entity);

		m_view = new ui::CameraView2DCtrl(Vector2<int_canvas>(0, 0), Vector2<int_canvas>(640, 480));
		m_view->SetCamera(m_camera);
		SetCurrentViewport(m_view);

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

		Image::Init();
		Font::Init();

		m_font = Font::CreateFromFile("../res/FreeMono.ttf");
		if(m_font)
			m_font->SetCharSize(0, 16 * 64, 96, 96);

		game2d::PropDef::LoadFromFile("../res/sampleProps.txt");

		m_editorCtrl = new ui::Editor2DCtrl(Vector2<int_canvas>(0, 0), Vector2<int_canvas>(m_canvas->GetWidth(), m_canvas->GetHeight()), m_font);

		bool parentStart = game2d::AppGame2D::OnStart();
		if(!parentStart)
			return false;
		
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
		dif *= 512.f;
		m_entity->SetVelocity(dif);

/*		Vector2<float> origin = Vector2<float>(m_entity->GetPosition()[0] + 16.f, m_entity->GetPosition()[1] + 16.f);
//		origin = Vector2<float>(512.f, 512.f);

		vector<Group::Member *> members = m_group->GetMembers();
		for(vector<Group::Member *>::iterator it = members.begin(); it != members.end(); it++)
		{
			game2d::PhysicalObject *obj = (game2d::PhysicalObject *)*it;
			if(obj == m_entity)
				continue;
			Vector2<float> pos = obj->GetPosition();
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
			obj->SetVelocity(vel);
		}*/

		game2d::Entity::FinalizeDelete();
		m_group->ProcessPhysics(dt);
		game2d::Entity::Process(dt);
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

		sleepMS(1);
		return true;
	}
	void OnStopped()
	{
		m_physicsThread->Join();

		m_group->DetachHandler();
		delete m_box2dPhysicsHandler;
	
		RenderPrimitiveCleanup();
		game2d::PropDef::Cleanup();

		delete m_editorCtrl;
		delete m_view;
		delete m_camera;
		game2d::Entity::FinalizeDelete();
		delete m_entity;
		for(int a = 0; a < NUMRANDOMS; a++)
			delete m_randoms[a];
		delete [] m_randoms;
		delete m_group;
		delete m_font;
		delete m_canvas;

		game2d::AppGame2D::OnStopped();
	}
	bool OnEvent(Event &event)
	{
		switch(event.type)
		{
		case event::KeyDown:
			if(event.key.keyCode == Key_F10)
				m_isEditMode = !m_isEditMode;
			if(m_isEditMode)
				m_editorCtrl->OnEvent(event);
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
			if(m_isEditMode)
				m_editorCtrl->OnEvent(event);
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
		case event::MouseMotion:
			if(m_isEditMode)
				m_editorCtrl->OnEvent(event);
			break;
		case event::MouseButtonDown:
			if(m_isEditMode)
				m_editorCtrl->OnEvent(event);
			break;
		case event::MouseButtonUp:
			if(m_isEditMode)
				m_editorCtrl->OnEvent(event);
			break;
		case event::PostRender:
			LockWorldMutex();
			if(m_view)
				m_view->Render(m_canvas);
			if(m_isEditMode)
				if(m_editorCtrl)
					m_editorCtrl->Render(m_canvas);
			UnlockWorldMutex();
			break;
		case event::WindowResize:
			if(m_view)
				m_view->SetExtent(Vector2<int_canvas>(event.windowResize.width, event.windowResize.height));
			if(m_editorCtrl)
				m_editorCtrl->SetExtent(Vector2<int_canvas>(event.windowResize.width, event.windowResize.height));
			break;
		}
		return true;
	}
	Vector2<float> GetWorldPositionFromCanvasPosition(int_canvas x, int_canvas y)
	{
		Vector2<int_canvas> extent = m_view->GetExtent();
		Vector2<float> viewScale = m_view->GetViewScale();
		Vector2<float> dif((float)(x - extent[0] / 2) / viewScale[0], (float)(extent[1] / 2 - y) / viewScale[1]);
		return dif + m_entity->GetPosition();
	}
};

void *physicsFunc(void *arg)
{
	App2DEditorSample *app = (App2DEditorSample *)arg;
	unsigned long long lastProcess = app->GetRunTimeMS();

	while(app->IsRunning())
	{
		unsigned long long t = app->GetRunTimeMS();
		if((t - lastProcess) > 15)
		{
			float dt = (float)(t - lastProcess) / 1000.f;
			lastProcess = t;

			app->LockWorldMutex();
			app->ProcessPhysics(dt);
			app->UnlockWorldMutex();
		}

		sleepMS(1);
	}
	
	Thread::Exit(NULL);
	return 0;
}

int main(int argc, char **argv)
{
	print("530 - 2D Editor | Centhra Engine v%s\n", getVersionString().c_str());

	App2DEditorSample myApp;
	myApp.Start();

	while(myApp.IsRunning())
		myApp.Process();

	return 0;
}
