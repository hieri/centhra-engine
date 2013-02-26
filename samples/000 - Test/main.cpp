//- Centhra Engine -
#include <CE/AppFrontend.h>
#include <CE/Base.h>
#include <CE/Canvas.h>
#include <CE/Game2D/Plane.h>
#include <CE/Game2D/ZoneEntity.h>
#include <CE/Game2D/ZoneCamera.h>
#include <CE/UI/GameView2D.h>

//- Standard Library -
#include <stdlib.h>

//- OpenGL -
#include <GL/gl.h>
#include <GL/glu.h>

using namespace ce;

#define NUMRANDOMS 250

//- Define your own implementation of the AppFrontend class. -
class AppTest : public AppFrontend
{
	Canvas *m_canvas;
	game2d::Plane *m_plane;
	game2d::ZoneEntity *m_entity;
	game2d::ZoneCamera *m_camera;
	ui::GameView2D *m_view;
	bool w,a,s,d;
	game2d::ZoneEntity **m_randoms;

public:
	AppTest()
	{
		m_canvas = 0;
		m_plane = 0;
		m_entity = 0;
		m_camera = 0;
		m_view = 0;
		w = a = s = d = false;
		m_randoms = 0;
	}

	//- Define the virtual functions for the class. -
	bool OnStart()
	{
		srand(GetRunTimeMS());
		m_canvas = Canvas::Create(640, 480, "000 - Test");
		m_plane = new game2d::Plane(16, 16, 64.f);
		m_entity = new game2d::ZoneEntity(Vector2<float>(128.f, 128.f), Vector2<float>(64.f, 64.f));
		m_plane->Place(m_entity);
		
		m_camera = new game2d::ZoneCamera();
		m_camera->SetFocus(m_entity);

		m_view = new ui::GameView2D(Vector2<int>(0, 0), Vector2<int>(640, 480));
		m_view->SetCamera(m_camera);

		m_randoms = new game2d::ZoneEntity *[NUMRANDOMS];
		for(int a = 0; a < NUMRANDOMS; a++)
		{
			m_randoms[a] = new game2d::ZoneEntity(Vector2<float>((float)(rand() % 1024), (float)(rand() % 1024)), Vector2<float>(16.f, 16.f));
			m_plane->Place(m_randoms[a]);
		}

		return true;
	}
	bool OnProcess()
	{
		sleepMS(1);
		return true;
	}
	bool OnStop(bool force)
	{
		delete m_canvas;
		delete m_plane;
		delete m_entity;
		delete m_camera;
		delete m_view;
		for(int a = 0; a < NUMRANDOMS; a++)
			delete m_randoms[a];
		delete [] m_randoms;

		return true;
	}
	bool OnEvent(Event &event)
	{
		switch(event.type)
		{
			case event::KeyDown:
				switch(event.key.keyCode)
				{
					case 25:
						w = true;
						break;
					case 38:
						a = true;
						break;
					case 39:
						s = true;
						break;
					case 40:
						d = true;
						break;
				}
				break;
			case event::KeyUp:
				switch(event.key.keyCode)
				{
					case 25:
						w = false;
						break;
					case 38:
						a = false;
						break;
					case 39:
						s = false;
						break;
					case 40:
						d = false;
						break;
				}
				break;
			case event::PostRender:
				Vector2<float> dif;
				if(w)
					dif[1] += 5.2f;
				if(a)
					dif[0] -= 5.2f;
				if(s)
					dif[1] -= 5.2f;
				if(d)
					dif[0] += 5.2f;

				m_entity->Move(dif);

				for(int a = 0; a < NUMRANDOMS; a++)
				{
					dif = Vector2<float>(0.f, 0.f);
					unsigned char mov = rand() % 16;
					if(mov & 1)
						dif[1] += 1.f;
					if(mov & 2)
						dif[0] -= 1.f;
					if(mov & 4)
						dif[1] -= 1.f;
					if(mov & 8)
						dif[0] += 1.f;
					Vector2<float> pos = m_randoms[a]->GetPosition();
					if(pos[0] > 1024.f && dif[0] > 0)
						dif[0] *= -1.f;
					if(pos[0] < 0.f && dif[0] < 0)
						dif[0] *= -1.f;
					if(pos[1] > 1024.f && dif[1] > 0)
						dif[1] *= -1.f;
					if(pos[1] < 0.f && dif[1] < 0)
						dif[1] *= -1.f;
					m_randoms[a]->Move(dif);
				}

				m_view->Render();
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
