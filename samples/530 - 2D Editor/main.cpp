//- Standard Library -
#include <cstdlib>
#include <ctime>
#include <cmath>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Thread.h>
#include <CE/Frontend.h>
#include <CE/Counter.h>
#include <CE/Game2D.h>
#include <CE/Game2D/DefaultPhysicsHandler.h>
#include <CE/Plugin/Box2D/PhysicsHandler.h>
#include <CE/Plugin/Tiled/TMX.h>
#include <CE/UI/Editor2D.h>
#include <CE/Math.h>
#include <CE/Renderer.h>

#include <CE/UI/ImageCtrl.h>

using namespace ce;
using namespace std;

#define NUMRANDOMS 32

void *physicsFunc(void *arg);

//#define RENDER_PROFILE

class App2DEditorSample : public AppGame2D
{
	Canvas *m_canvas;
	game2d::World *m_world;
	game2d::PhysicalObject *m_entity, **m_randoms;
	game2d::Camera *m_camera;
	ui::CameraView2DCtrl *m_view;
	game2d::DefaultPhysicsHandler *m_defaultPhysicsHandler;
	plugin::box2d::bPhysicsHandler *m_box2dPhysicsHandler;
	ce::Font *m_font;
	bool m_isEditMode;
	bool w,a,s,d;
	unsigned long long m_lastProcess;

	Counter m_fpsCounter;
	ui::TextCtrl *m_fpsDisplay;

	float m_zoomLevel;

	plugin::tiled::TMX *m_tmx;

	ui::Editor2DCtrl *m_editorCtrl;
	Image *m_editorScrollImage;
	ui::Skin *m_editorScrollSkin;

public:
	Thread* m_physicsThread;

	App2DEditorSample() : AppGame2D(), m_fpsCounter(1000), m_fpsDisplay(0)
	{
		m_canvas = 0;
		m_defaultPhysicsHandler = 0;
		m_box2dPhysicsHandler = 0;
		m_entity = 0;
		m_camera = 0;
		m_world = 0;
		m_view = 0;
		m_lastProcess = 0;
		w = a = s = d = false;
		m_randoms = 0;
		m_editorCtrl = 0;
		m_font = 0;
		m_isEditMode = false;
		m_physicsThread = new Thread(&physicsFunc);

		m_zoomLevel = 1.f;

		m_tmx = 0;
	}
	~App2DEditorSample()
	{
		delete m_physicsThread;
	}
	bool OnStart()
	{
		srand((unsigned int)time(NULL));

		m_canvas = Canvas::Create(640, 480, "530 - 2D Editor");
		#ifdef RENDER_PROFILE
			m_canvas->SetVSync(false);
		#endif

		Image::Init();
		ce::Font::Init();

		m_world = new game2d::World();

		m_font = ce::Font::CreateFromFile("../res/FreeMono.ttf", 16, m_canvas->GetHorizontalDPI(), m_canvas->GetVerticalDPI());

		game2d::PropDef::LoadFromFile("../res/sampleProps.txt");
		game2d::ProjectileDef::LoadFromFile("../res/sampleProjectiles.txt");
		game2d::ExplosionDef::LoadFromFile("../res/sampleExplosions.txt");

		m_tmx = plugin::tiled::TMX::CreateFromFile("../res/Test.tmx");
		m_tmx->PopulateWorld(m_world, Vector2<float>(32.f, 32.f));

//		Directory::GetFromPath("../res/")->SetMonitoring(true);

		game2d::World::ObjectLayer *objectLayerA = (game2d::World::ObjectLayer *)m_world->GetLayer(1);

/*		game2d::World::ObjectLayer *objectLayerA = m_world->AddObjectLayer();
		game2d::World::ObjectLayer *objectLayerB = m_world->AddObjectLayer();*/

		m_entity = new game2d::PhysicalObject(Vector2<float>(512.f, 512.f), Vector2<float>(32.f, 32.f));
		m_world->Add(m_entity);
		m_entity->SetRenderLayer(objectLayerA);

		m_camera = new game2d::Camera(m_world);
		m_camera->SetFocus(m_entity);

		m_view = new ui::CameraView2DCtrl(Vector2<int_canvas>(0, 0), Vector2<int_canvas>(640, 480));
		m_view->SetCamera(m_camera);
		SetCurrentViewport(m_view);

		m_view->SetViewScale(Vector2<float>(m_zoomLevel, m_zoomLevel));

//		m_entity->SetCollisionMask(0);

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
			m_world->Add(m_randoms[a]);
			m_randoms[a]->SetRenderLayer(objectLayerA);
		}

		m_box2dPhysicsHandler = new plugin::box2d::bPhysicsHandler();
		m_world->AttachHandler(m_box2dPhysicsHandler);
//		m_defaultPhysicsHandler = new game2d::DefaultPhysicsHandler();
//		m_world->AttachHandler(m_defaultPhysicsHandler);

		m_editorScrollImage = Image::CreateFromFile("../res/editorPropScroll.png");
		m_editorScrollSkin = new ui::Skin(m_editorScrollImage);
		m_fpsDisplay = new ui::TextCtrl(Vector2<int_canvas>(0, 0), Vector2<int_canvas>(256, 32), m_font);
		m_editorCtrl = new ui::Editor2DCtrl(Vector2<int_canvas>(0, 0), Vector2<int_canvas>(m_canvas->GetWidth(), m_canvas->GetHeight()), m_font, m_editorScrollSkin);

		bool parentStart = AppGame2D::OnStart();
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
		if(m_camera->GetMode() == game2d::Camera::Mode_Follow)
			m_entity->SetVelocity(dif);
		else
			m_camera->SetPosition(m_camera->GetPosition() + dif * dt / m_zoomLevel);

		game2d::Entity::FinalizeDelete();
		m_world->ProcessPhysics(dt);
		game2d::Entity::Process(dt);
	}
	bool OnProcess()
	{
		#ifndef RENDER_PROFILE
			sleepMS(1);
		#endif
		return true;
	}
	void OnStopped()
	{
		m_physicsThread->Join();

		m_world->DetachHandler();
		delete m_box2dPhysicsHandler;
//		delete m_defaultPhysicsHandler;
	
		delete m_fpsDisplay;
		delete m_editorCtrl;
		delete m_view;
		delete m_camera;

		delete m_world;

		game2d::Entity::FinalizeDelete();

		delete m_tmx;

		RenderPrimitiveCleanup();
		game2d::PropDef::Cleanup();
		Image::Cleanup();
		File::Cleanup();

		delete m_editorScrollSkin;
		delete m_editorScrollImage;
		delete m_font;

		delete m_canvas;
		AppGame2D::OnStopped();
	}
	unsigned long g_fpsCount = 0;
	bool OnEvent(Event &event)
	{
		switch(event.type)
		{
		case event::KeyDown:
			if(event.key.keyCode == Key_F10)
			{
				m_isEditMode = !m_isEditMode;
				if(m_isEditMode)
					m_camera->SetPosition(m_entity->GetPosition());
				else
				{
					m_zoomLevel = 1.f;
					if(m_view)
						m_view->SetViewScale(Vector2<float>(m_zoomLevel, m_zoomLevel));
				}
				m_camera->SetMode(m_isEditMode ? game2d::Camera::Mode_Free : game2d::Camera::Mode_Follow);
			}
			else if(event.key.keyCode == Key_F11)
				m_canvas->SetFullscreen(!m_canvas->IsFullscreen());
			else if(event.key.keyCode == Key_F1)
			{
				Image *screen = Image::CreateFromScreen(Vector2<unsigned int>(0, 0), Vector2<unsigned int>(m_canvas->GetWidth(), m_canvas->GetHeight()));
				screen->SaveToFile("screenshot.png");
				print("Screenshot saved to \'screenshot.png\'.\n");
				delete screen;
			}
			if(m_isEditMode)
				m_editorCtrl->ProcessEvent(event);
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
			case Key_BracketRight:
				m_tmx->ExportWorldToFile(m_world, "../res/Test.tmx");
				break;
			}
			break;
		case event::KeyUp:
			if(m_isEditMode)
				m_editorCtrl->ProcessEvent(event);
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
		case event::MouseScroll:
			if(m_isEditMode)
				if(m_editorCtrl->ProcessEvent(event))
				{
					if(!event.mouseScroll.isHorizontal)
					{
						m_zoomLevel *= 1.f + ((float)event.mouseScroll.delta / 120.f) / 8.f;
						if(m_view)
							m_view->SetViewScale(Vector2<float>(m_zoomLevel, m_zoomLevel));
					}
				}
			break;
		case event::MouseMotion:
		case event::MouseButtonUp:
			if(m_isEditMode)
				m_editorCtrl->ProcessEvent(event);
			break;
		case event::MouseButtonDown:
			if(m_isEditMode)
				m_editorCtrl->ProcessEvent(event);
			else
			{
				Vector2<float> pos = GetWorldPositionFromCanvasPosition(event.mouseButton.x, event.mouseButton.y);
				Vector2<float> dir = pos - m_entity->GetPosition();
				dir /= dir.GetLength();
				LockWorldMutex();
				if(event.mouseButton.button == event::MouseButtonLeft || event.mouseButton.button == event::MouseButtonRight)
				{
					game2d::Projectile *p = 0;
					if(event.mouseButton.button == event::MouseButtonLeft)
						p = game2d::ProjectileDef::GetDefByName("blueBall")->Spawn(pos, dir, 0, RadiansToDegrees() * atan2(dir[1], dir[0]));
					else
						p = game2d::ProjectileDef::GetDefByName("bullet")->Spawn(pos, dir, 0, RadiansToDegrees() * atan2(dir[1], dir[0]));
					m_world->Add(p);
				}
				else
				{
					game2d::Explosion *e = game2d::ExplosionDef::GetDefByName("boom")->Spawn(pos);
					m_world->Add(e);
				}
				UnlockWorldMutex();
//				print("P: %f %f\t%f %f\n", p->GetPosition()[0], p->GetPosition()[1], p->GetVelocity()[0], p->GetVelocity()[1]);
			}
			break;
		case event::PostRender:
			LockWorldMutex();
			if(m_view)
				m_view->Render(m_canvas);
			if(m_isEditMode)
				if(m_editorCtrl)
					m_editorCtrl->Render(m_canvas);
			if(m_fpsDisplay)
			{
				m_fpsCounter.Increment();
				if(m_fpsCounter.PopCount(event.base.timeMS, g_fpsCount))
					m_fpsDisplay->SetText("%d", g_fpsCount);
				m_fpsDisplay->Render(m_canvas);
			}
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
