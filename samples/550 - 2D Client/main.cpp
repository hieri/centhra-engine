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
#include <CE/Plugin/Box2D/PhysicsHandler.h>

//- Standard Library -
#include <cstdlib>
#include <string>
#include <ctime>
#include <cstring>
#include <iostream>

using namespace ce;
using namespace std;

void *physicsFunc(void *arg);
void *connectionFunc(void *arg);
Mutex g_physicsMutex;

typedef enum PacketType
{
	Unknown,
	Movement,
	Update,
	New,
	Delete,
	Control
} PacketType;
typedef struct NewPacket
{
	unsigned short type, objID;
	float posX, posY;
} NewPacket;
typedef struct DeletePacket
{
	unsigned short type, objID;
} DeletePacket;
typedef struct MovementPacket
{
	unsigned short type, objID;
	float velX, velY;
} MovementPacket;
typedef struct UpdatePacket
{
	unsigned short type, objID;
	float posX, posY, velX, velY, rot;
} UpdatePacket;
typedef struct ControlPacket
{
	unsigned short type, objID;
} ControlPacket;
typedef union Packet
{
	unsigned short type;
	MovementPacket movement;
	UpdatePacket update;
	DeletePacket del;
	NewPacket n;
	ControlPacket control;
} Packet;

class App2DClientSample : public AppFrontend
{
	Canvas *m_canvas;
	ui::CameraView2DCtrl *m_view;
	game2d::DefaultPhysicsHandler *m_defaultPhysicsHandler;
	plugin::box2d::bPhysicsHandler *m_box2dPhysicsHandler;
	bool w,a,s,d;
	unsigned long long m_lastProcess;

public:
	game2d::PhysicalObject *m_entity, *m_dummy;
	game2d::PhysicalGroup *m_group;
	Thread *m_physicsThread, *m_connectionThread;
	Vector2<float> m_targetMotion;
	game2d::Camera *m_camera;

	App2DClientSample()
	{
		m_canvas = 0;
		m_defaultPhysicsHandler = 0;
		m_box2dPhysicsHandler = 0;
		m_entity = m_dummy = 0;
		m_camera = 0;
		m_group = 0;
		m_view = 0;
		m_lastProcess = 0;
		w = a = s = d = false;
		m_physicsThread = new Thread(&physicsFunc);
		m_connectionThread = new Thread(&connectionFunc);
	}
	~App2DClientSample()
	{
		delete m_physicsThread;
		delete m_connectionThread;
	}
	bool OnStart()
	{
		m_canvas = Canvas::Create(320, 240, "550 - 2D Client");

		m_group = new game2d::PhysicalGroup();

		m_dummy = new game2d::PhysicalObject(Vector2<float>(460.f, 512.f), Vector2<float>(32.f, 32.f));
		m_group->Add(m_dummy);

		m_camera = new game2d::Camera();

		m_view = new ui::CameraView2DCtrl(Vector2<int>(0, 0), Vector2<int>(320, 240));
		m_view->SetCamera(m_camera);

//		m_defaultPhysicsHandler = new game2d::DefaultPhysicsHandler();
//		m_group->AttachHandler(m_defaultPhysicsHandler);

		m_box2dPhysicsHandler = new plugin::box2d::bPhysicsHandler();
		m_group->AttachHandler(m_box2dPhysicsHandler);

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
		m_targetMotion = dif;

		g_physicsMutex.Lock();
		game2d::Entity::FinalizeDelete();
		if(m_entity)
			m_entity->SetVelocity(dif);
		m_group->ProcessPhysics(dt);
		g_physicsMutex.Unlock();
	}
	bool OnProcess()
	{
		unsigned long long t = GetRunTimeMS();
		if((t - m_lastProcess) > 15)
		{
			float dt = (float)(t - m_lastProcess) / 1000.f;
			m_lastProcess = t;

//			ProcessPhysics(dt);
		}

		sleepMS(1);
		return true;
	}
	void OnStopped()
	{
		m_physicsThread->Join();
		m_connectionThread->Join();

		m_group->DetachHandler();
		delete m_box2dPhysicsHandler;
		delete m_defaultPhysicsHandler;

		g_physicsMutex.Destroy();

		delete m_dummy;
		delete m_view;
		delete m_camera;
		game2d::Entity::FinalizeDelete();
		delete m_entity;
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
	App2DClientSample *app = (App2DClientSample *)arg;
	unsigned long long lastMovement, lastConnection;
	lastMovement = lastConnection = app->GetRunTimeMS();

	Socket *client = Socket::Create(Socket::IP4, Socket::Stream, Socket::TCP);
	string connectionMsg = "2dClient";
	string packetPrefix = "P:";

	bool isConnected = false;
	unsigned short packetSize = sizeof(Packet);
	string readBuffer;
	int hasRead = 0;
	while(app->IsRunning())
	{
		unsigned long long t = app->GetRunTimeMS();
		if(isConnected)
		{
			if((t - lastMovement) > 16)
			{
				lastMovement = t;

				g_physicsMutex.Lock();
				Vector2<float> velocity = app->m_targetMotion;
				Packet movement;
				movement.type = Movement;
				movement.movement.velX = velocity[0];
				movement.movement.velY = velocity[1];
				g_physicsMutex.Unlock();
				string tempPacket(packetPrefix);
				tempPacket.append((char *)&movement.type, sizeof(unsigned short));
				tempPacket.append((char *)&movement, sizeof(MovementPacket));
				client->Write((char *)tempPacket.c_str(), tempPacket.size());
			}

			hasRead = client->HasRead();
			if(hasRead > 0)
			{
				do
				{
					char buffer[257];
					memset(buffer, 0, 257);
					int ret = client->Read(buffer, 256);
					if(ret > 0)
						readBuffer.append(buffer, ret);
				}
				while(client->HasRead() > 0);
			}

			while(readBuffer.size() >= packetSize)
			{
//				cout << "READ" << endl;
				size_t fP = readBuffer.find_first_of("P:");
				if(fP != 0)
				{
					if(fP == string::npos)
						readBuffer.clear();
					else
						readBuffer = readBuffer.substr(fP);
				}

				if(readBuffer.size() < packetSize)
					break;

				unsigned short type = 0;
				memcpy(&type, &readBuffer[2], sizeof(unsigned short));

				unsigned short thisPacketSize = 0;

				switch(type) //TODO: Refer to array
				{
				case Movement:
					thisPacketSize = sizeof(MovementPacket);
					break;
				case Update:
					thisPacketSize = sizeof(UpdatePacket);
					break;
				case New:
					thisPacketSize = sizeof(NewPacket);
					break;
				case Delete:
					thisPacketSize = sizeof(DeletePacket);
					break;
				case Control:
					thisPacketSize = sizeof(ControlPacket);
					break;
				}

				Packet response;
				memcpy(&response, &readBuffer[4], thisPacketSize);
//				print("Packet: %s\n", readBuffer.c_str());

//				print("Packet: %d\n", response.type);
//				cout << "Packet: " << response.type << endl;

				if(response.type == Update)
				{
//					print("Update: %f %f\n", response.update.posX, response.update.posY);
//					cout << "Update: " << response.update.posX << " " << response.update.posY << endl;
					g_physicsMutex.Lock();
					game2d::PhysicalObject *obj = game2d::PhysicalObject::GetNetObjectByID(response.update.objID);
					if(obj)
					{
						obj->SetPosition(Vector2<float>(response.update.posX, response.update.posY));
						obj->SetVelocity((Vector2<float>(response.update.velX, response.update.velY)));
						obj->SetRotation(response.update.rot);
					}
					g_physicsMutex.Unlock();
				}
				else if(response.type == Control) //- Needs to handle multithreaded stdio properly, in order to debug this -
				{
					print("CONTROL %d\n", response.control.objID);
					g_physicsMutex.Lock();
					game2d::PhysicalObject *obj = game2d::PhysicalObject::GetNetObjectByID(response.control.objID);
					if(obj)
					{
						app->m_entity = obj;
						app->m_camera->SetFocus(obj);
					}
					g_physicsMutex.Unlock();
				}
				else if(response.type == New) //- Needs to handle multithreaded stdio properly, in order to debug this -
				{
					print("NEW %d %f %f\n", response.n.objID, response.n.posY, response.n.posY);
					g_physicsMutex.Lock();
					game2d::PhysicalObject *obj = new game2d::PhysicalObject(Vector2<float>(response.n.posX, response.n.posY), Vector2<float>(32.f, 32.f), game2d::PhysicalObject::DEFAULT_ID, response.n.objID);
					app->m_group->Add(obj);
					g_physicsMutex.Unlock();
				}
				else if(response.type == Delete)
				{
					print("DEL %d\n", response.del.objID);
					g_physicsMutex.Lock();
					game2d::PhysicalObject *obj = game2d::PhysicalObject::GetNetObjectByID(response.del.objID);
					if(obj)
					{
						app->m_group->Remove(obj);
						obj->Delete();
					}
					g_physicsMutex.Unlock();
				}

				readBuffer = readBuffer.substr(thisPacketSize + 4);				
			} 
		}
		else
		{
			if((t - lastConnection) > 16)
			{
				lastConnection = t;
				if(client->Connect("127.0.0.1", 12346))
				{
					isConnected = true;
					client->Write((char *)connectionMsg.c_str(), connectionMsg.length());
					cout << "Connected" << endl;
				}
				else
				{
					cout << "Stopping..." << endl;
					app->Stop(); //TODO: Handle this better
				}
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
	App2DClientSample *app = (App2DClientSample *)arg;
	unsigned long long lastProcess = app->GetRunTimeMS();

	cout << "Physics Func" << endl;
	while(app->IsRunning())
	{
		unsigned long long t = app->GetRunTimeMS();
		if((t - lastProcess) > 15)
		{
			float dt = (float)(t - lastProcess) / 1000.f;
			lastProcess = t;

			app->ProcessPhysics(dt);
		}

		sleepMS(1);
	}

	Thread::Exit(NULL);
	return 0;
}

int main(int argc, char **argv)
{
	ioMutexInit();
	print("550 - 2D Client | Centhra Engine v%s\n", getVersionString().c_str());

	App2DClientSample myApp;
	myApp.Start();

	while(myApp.IsRunning())
		myApp.Process();

	ioMutexDestroy();
	return 0;
}
