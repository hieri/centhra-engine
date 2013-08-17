//- Centhra Engine -
#include <CE/AppFrontend.h>
#include <CE/Base.h>
#include <CE/Canvas.h>
#include <CE/Game2D/Graph.h>
#include <CE/Game2D/Camera.h>
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Game2D/PhysicalGroup.h>
#include <CE/Game2D/DefaultPhysicsHandler.h>
#include <CE/UI/CameraView2DCtrl.h>

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

class NodeEntity : public game2d::PhysicalObject
{
	game2d::Graph::Node *m_node;

protected:
	void DoRender()
	{
		Vector2<float> position = GetPosition();
		Vector2<float> extent = GetExtent();

		glPushMatrix();
			glTranslatef(position[0], position[1], 0.f);
			glScalef(extent[0], extent[1], 1.f);
			glBegin(GL_LINE_LOOP);
				glVertex2i(0, 0);
				glVertex2i(1, 0);
				glVertex2i(1, 1);
				glVertex2i(0, 1);
				glVertex2i(0, 0);
			glEnd();
		glPopMatrix();
		
		glBegin(GL_LINES);
		std::vector<game2d::Graph::Node *> vec = m_node->GetNeighbors();
		Vector2<float> origin = m_node->GetPosition();
		for(std::vector<game2d::Graph::Node *>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			Vector2<float> nodePos = (*it)->GetPosition();
			glVertex2f(origin[0], origin[1]);
			glVertex2f(nodePos[0], nodePos[1]);
		}
		glEnd();
	}

public:
	NodeEntity(game2d::Graph::Node *node) : PhysicalObject(node->GetPosition() - Vector2<float>(8.f, 8.f), Vector2<float>(16.f, 16.f))
	{
		m_node = node;
		m_collisionMask = 0;
	}
};

class AppTest : public AppFrontend
{
	Canvas *m_canvas;
	game2d::PhysicalGroup *m_group;
	game2d::PhysicalObject *m_entity, *m_walls[3];
	game2d::Camera *m_camera;
	ui::CameraView2DCtrl *m_view;
	game2d::DefaultPhysicsHandler *m_defaultPhysicsHandler;
	Vector2<float> m_pointA, m_pointB;
	std::vector<Vector2<float> > m_path;
	NodeEntity *m_nodeEntities[10];
	game2d::Graph::Node *m_nodes[10];
	game2d::Graph *m_graph;
	bool w,a,s,d;
	unsigned long m_lastProcess;

public:

	AppTest()
	{
		m_canvas = 0;
		m_camera = 0;
		m_defaultPhysicsHandler = 0;
		m_view = 0;
		m_entity = 0;
		m_group = 0;
		m_lastProcess = 0;
		w = a = s = d = false;
		m_graph = 0;
	}
	~AppTest()
	{
	}

	void OnStarted()
	{
		srand(GetRunTimeMS());
		m_canvas = Canvas::Create(640, 480, "520 - 2D Graph Traversal");
		m_group = new game2d::PhysicalGroup();
		m_entity = new game2d::PhysicalObject(Vector2<float>(512.f, 512.f), Vector2<float>(32.f, 32.f));
		m_entity->SetCollisionMask(0);
		m_group->Add(m_entity);

		m_camera = new game2d::Camera();
		m_camera->SetFocus(m_entity);

		m_view = new ui::CameraView2DCtrl(Vector2<int>(0, 0), Vector2<int>(640, 480));
		m_view->SetCamera(m_camera);

		m_nodes[0] = new game2d::Graph::Node(ce::Vector2<float>(128.f, 128.f));
		m_nodes[1] = new game2d::Graph::Node(ce::Vector2<float>(256.f, 128.f));
		m_nodes[2] = new game2d::Graph::Node(ce::Vector2<float>(512.f, 128.f));
		m_nodes[3] = new game2d::Graph::Node(ce::Vector2<float>(256.f, 256.f));
		m_nodes[4] = new game2d::Graph::Node(ce::Vector2<float>(512.f, 256.f));
		m_nodes[5] = new game2d::Graph::Node(ce::Vector2<float>(128.f, 512.f));
		m_nodes[6] = new game2d::Graph::Node(ce::Vector2<float>(128.f, 256.f));
		m_nodes[7] = new game2d::Graph::Node(ce::Vector2<float>(128.f, 512.f));
		m_nodes[8] = new game2d::Graph::Node(ce::Vector2<float>(384.f, 256.f));
		m_nodes[9] = new game2d::Graph::Node(ce::Vector2<float>(384.f, 512.f));
		m_graph = new game2d::Graph();

		m_pointB = ce::Vector2<float>(256.f, 128.f);
		m_pointA = ce::Vector2<float>(128.f, 256.f);

		m_walls[0] = new game2d::PhysicalObject(ce::Vector2<float>(144.f, 144.f), ce::Vector2<float>(100.f, 100.f));
		m_walls[1] = new game2d::PhysicalObject(ce::Vector2<float>(270.f, 144.f), ce::Vector2<float>(230.f, 100.f));
		m_walls[2] = new game2d::PhysicalObject(ce::Vector2<float>(144.f, 270.f), ce::Vector2<float>(230.f, 225.f));

		for(int a = 0; a < 10; a++)
		{
			m_nodeEntities[a] = new NodeEntity(m_nodes[a]);
			m_graph->Add(m_nodes[a]);
			m_group->Add(m_nodeEntities[a]);
		}
		
		for(int a = 0; a < 3; a++)
			m_group->Add(m_walls[a]);

		m_graph->GenerateNeighbors(m_group, 1);

		m_pointA = Vector2<float>(0.f, 0.f);
		m_pointB = Vector2<float>(512.f, 512.f);
		if(m_group->SegmentSearch(m_pointA[0], m_pointA[1], m_pointB[0], m_pointB[1], 1).size())
			m_path = m_graph->FindPath(m_pointA, m_pointB, 1, m_group);
		
		m_defaultPhysicsHandler = new game2d::DefaultPhysicsHandler();
		m_group->AttachHandler(m_defaultPhysicsHandler);
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

			m_group->ProcessPhysics(dt);

//			m_plane->RemoveDead();
//			game2d::Entity::DeleteDead();
		}

		sleepMS(1);
		return true;
	}
	void OnStopped()
	{
		m_group->CleanupHandler();
		delete m_defaultPhysicsHandler;

		delete m_graph;
		for(int a = 0; a < 10; a++)
		{
			delete m_nodeEntities[a];
			delete m_nodes[a];
		}
		delete m_view;
		delete m_camera;
		for(int a = 0; a < 3; a++)
			delete m_walls[a];
		delete m_entity;
		delete m_group;
		game2d::Entity::DeleteDead();
//		game2d::ZoneEntity::Cleanup();
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
				m_path.clear();
				if(m_group->SegmentSearch(m_pointA[0], m_pointA[1], m_pointB[0], m_pointB[1], 1).size())
					m_path = m_graph->FindPath(m_pointA, m_pointB, 1, m_group);
				break;
			case 2:
				m_pointB = Vector2<float>(center[0] - halfExtent[0] + (float)event.mouseButton.x, center[1] + halfExtent[1] - (float)event.mouseButton.y);
				m_path.clear();
				if(m_group->SegmentSearch(m_pointA[0], m_pointA[1], m_pointB[0], m_pointB[1], 1).size())
					m_path = m_graph->FindPath(m_pointA, m_pointB, 1, m_group);
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
				glColor4ub(255, 0, 0, 255);
				glTranslatef((float)halfExtent[0], (float)halfExtent[1], 0.f);
				glTranslatef(-center[0], -center[1], 0.f);

				glBegin(GL_LINES);
					glVertex2f(m_pointA[0], m_pointA[1]);

					for(std::vector<Vector2<float> >::iterator it = m_path.begin(); it != m_path.end(); it++)
					{
						Vector2<float> point = *it;
						glVertex2f(point[0], point[1]);
						glVertex2f(point[0], point[1]);
					}

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
	print("520 - 2D Graph Traversal | Centhra Engine v%s\n", getVersionString().c_str());

	AppTest myApp;
	myApp.Start();

	//- Run the App's main loop. -
	while(myApp.IsRunning())
		myApp.Process();

	return 0;
}
