//- Centhra Engine -
#include <CE/AppFrontend.h>
#include <CE/Base.h>
#include <CE/Canvas.h>
#include <CE/Game2D/Plane.h>
#include <CE/Game2D/ZoneEntity.h>
#include <CE/Game2D/ZoneCamera.h>
#include <CE/UI/GameView2DCtrl.h>

//- Standard Library -
#include <stdlib.h>

#define NUMRANDOMS 64

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

using namespace ce;

class AppTest : public AppFrontend
{
	Canvas *m_canvas;
	game2d::ZoneEntity *m_entity, **m_randoms;
	game2d::ZoneCamera *m_camera;
	ui::GameView2DCtrl *m_view;
	Vector2<float> m_pointA, m_pointB;
	bool w,a,s,d;
	unsigned long m_lastProcess;
	unsigned int m_numFound;

public:
	game2d::Plane *m_plane;

	AppTest()
	{
		m_canvas = 0;
		m_plane = 0;
		m_camera = 0;
		m_view = 0;
		m_entity = 0;
		m_randoms = 0;
		m_lastProcess = 0;
		w = a = s = d = false;
		m_numFound = 0;
	}
	~AppTest()
	{
	}

	void OnStarted()
	{
		srand(GetRunTimeMS());
		m_canvas = Canvas::Create(640, 480, "510 - 2D Line Segment Search");
		m_plane = new game2d::Plane(16, 16, 64.f);
		m_entity = new game2d::ZoneEntity(Vector2<float>(512.f, 512.f), Vector2<float>(32.f, 32.f));
		m_plane->Place(m_entity);

		m_camera = new game2d::ZoneCamera();
		m_camera->SetFocus(m_entity);

		m_view = new ui::GameView2DCtrl(Vector2<int>(0, 0), Vector2<int>(640, 480));
		m_view->SetCamera(m_camera);

		m_randoms = new game2d::ZoneEntity *[NUMRANDOMS];
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

			m_randoms[a] = new game2d::ZoneEntity(Vector2<float>((float)rx * 16.f, (float)ry * 16.f), Vector2<float>(16.f, 16.f));
			m_plane->Place(m_randoms[a]);
		}
		m_pointA = Vector2<float>(256.f, 256.f);
		m_pointB = Vector2<float>(768.f, 768.f);
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

			for(int a = 0; a < NUMRANDOMS; a++)
			{
				Vector2<float> pos = m_randoms[a]->GetPosition();
				Vector2<float> vel = m_randoms[a]->GetVelocity();

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

			m_plane->ProcessPhysics(dt);
			m_numFound = m_plane->SegmentSearch(m_pointA[0], m_pointA[1], m_pointB[0], m_pointB[1]).size();

			m_plane->RemoveDead();
			game2d::Entity::DeleteDead();
		}

		sleepMS(1);
		return true;
	}
	void OnStopped()
	{
		delete m_view;
		delete m_camera;
		if(m_plane)
			delete m_plane;
		delete m_entity;
		for(int a = 0; a < NUMRANDOMS; a++)
			delete m_randoms[a];
		delete [] m_randoms;
		game2d::Entity::DeleteDead();
		game2d::ZoneEntity::Cleanup();
		delete m_canvas;
	}

	bool OnEvent(Event &event)
	{
		Vector2<int> halfExtent;
		Vector2<float> center;
		switch(event.type)
		{
		case event::MouseButtonDown:
			halfExtent = m_view->GetExtent() / 2;
			center = m_entity->GetPosition() + m_entity->GetExtent() / 2.f;

			switch(event.mouseButton.button)
			{
			case 1:
				m_pointA = Vector2<float>(center[0] - halfExtent[0] + (float)event.mouseButton.x, center[1] + halfExtent[1] - (float)event.mouseButton.y);
				break;
			case 2:
				m_pointB = Vector2<float>(center[0] - halfExtent[0] + (float)event.mouseButton.x, center[1] + halfExtent[1] - (float)event.mouseButton.y);
				break;
			}
			break;
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
			halfExtent = m_view->GetExtent() / 2;
			center = m_entity->GetPosition() + m_entity->GetExtent() / 2.f;

			m_view->Render();
			glPushMatrix();
				if(m_numFound)
					glColor4ub(0, 255, 0, 255);
				glTranslatef((float)halfExtent[0], (float)halfExtent[1], 0.f);
				glTranslatef(-center[0], -center[1], 0.f);
				glBegin(GL_LINES);
					glVertex2f(m_pointA[0], m_pointA[1]);
					glVertex2f(m_pointB[0], m_pointB[1]);
				glEnd();
				glColor4ub(255, 255, 255, 255);
			glPopMatrix();
			break;
		case event::WindowResize:
			if(m_view)
				m_view->SetExtent(Vector2<int>(event.windowResize.width, event.windowResize.height));
			break;
		}
		return true;
	}
};

int main(int argc, char **argv)
{
	print("510 - 2D Line Segment Search | Centhra Engine v%s\n", getVersionString().c_str());

	AppTest myApp;
	myApp.Start();

	//- Run the App's main loop. -
	while(myApp.IsRunning())
		myApp.Process();

	return 0;
}
