//- Standard Library -
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <iostream>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Socket.h>
#include <CE/Thread.h>
#include <CE/Mutex.h>
#include <CE/AppFrontend.h>
#include <CE/Canvas.h>
#include <CE/Game2D/Camera.h>
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Game2D/PhysicalGroup.h>
#include <CE/Game2D/DefaultPhysicsHandler.h>
#include <CE/UI/CameraView2DCtrl.h>
#include <CE/Plugin/Box2D/PhysicsHandler.h>

using namespace ce;
using namespace std;

typedef enum PacketType
{
	Unknown,
	Movement,
	Update,
	New,
	Delete
} PacketType;
typedef struct NewPacket
{
	unsigned short type;
	unsigned long objID;
	float posX, posY;
} NewPacket;
typedef struct DeletePacket
{
	unsigned short type;
	unsigned long objID;
} DeletePacket;
typedef struct MovementPacket
{
	unsigned short type;
	unsigned long objID;
	float velX, velY;
} MovementPacket;
typedef struct UpdatePacket
{
	unsigned short type;
	unsigned long objID;
	float posX, posY, velX, velY, rot;
} UpdatePacket;
typedef union Packet
{
	unsigned short type;
	MovementPacket movement;
	UpdatePacket update;
	DeletePacket del;
	NewPacket n;
	char padding[60];
} Packet;

Mutex g_physicsMutex;

class ClientConnection;

class AppTest : public AppFrontend
{
	Canvas *m_canvas;
	game2d::Camera *m_camera;
	ui::CameraView2DCtrl *m_view;
	unsigned long m_lastProcess;

public:
	game2d::PhysicalGroup *m_group;
	game2d::DefaultPhysicsHandler *m_defaultPhysicsHandler;
	plugin::box2d::bPhysicsHandler *m_box2dPhysicsHandler;
	map<unsigned long, ClientConnection *> m_clientConnectionMap;

	AppTest()
	{
		m_group = 0;
		m_defaultPhysicsHandler = 0;
		m_box2dPhysicsHandler = 0;
		m_canvas = 0;
		m_camera = 0;
		m_view = 0;
		m_lastProcess = 0;
	}
	~AppTest()
	{
	}

	//- Define the virtual functions for the class. -
	bool OnStart()
	{
		m_canvas = Canvas::Create(640, 480, "551- 2D Server");

		m_group = new game2d::PhysicalGroup();
//		m_defaultPhysicsHandler = new game2d::DefaultPhysicsHandler();
//		m_group->AttachHandler(m_defaultPhysicsHandler);

		m_box2dPhysicsHandler = new plugin::box2d::bPhysicsHandler();
		m_group->AttachHandler(m_box2dPhysicsHandler);

		m_camera = new game2d::Camera();

		m_view = new ui::CameraView2DCtrl(Vector2<int>(0, 0), Vector2<int>(640, 480));
		m_view->SetCamera(m_camera);
		return true;
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
		delete m_box2dPhysicsHandler;
		delete m_group;

		delete m_view;
		delete m_camera;
		delete m_canvas;
	}

	bool OnEvent(Event &event)
	{
		switch(event.type)
		{
		case event::PostRender:
//			g_physicsMutex.Lock();
//			m_view->Render();
//			g_physicsMutex.Unlock();
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
	unsigned long long lastProcess = getRunTimeMS();

	AppTest *app = (AppTest *)arg;

	while(app->IsRunning())
	{
		unsigned long long t = getRunTimeMS();
		if((t - lastProcess) > 15)
		{
			float dt = (float)(t - lastProcess) / 1000.f;
			lastProcess = t;

			g_physicsMutex.Lock();
			game2d::Entity::DeleteDead();
			app->m_group->ProcessPhysics(dt);
			g_physicsMutex.Unlock();
		}

		sleepMS(1);
	}

	Thread::Exit();
	return 0;
}

class ClientConnection
{
public:
	unsigned long id;
	game2d::PhysicalObject *player;
	queue<pair<unsigned long, game2d::PhysicalObject *> > creationQueue;
	queue<unsigned long> deletionQueue;
};

// thinking of how to handle telling other clients about new's/delete's
// update also needs to include velocity
// detect when live connection fails
// write a generalized class for keeping track of networked objects
// get rid of need for a canvas with regards to the server
void *clientFunc(void *arg)
{
	srand(time(NULL));
	pair<AppTest *, Socket *> *data = (pair<AppTest *, Socket *> *)arg;
	Socket *client = data->second;
	AppTest *app = data->first;
	delete data;

	g_physicsMutex.Lock();
	game2d::PhysicalObject *player = new game2d::PhysicalObject(Vector2<float>(512.f, 512.f), Vector2<float>(32.f, 32.f));
	app->m_group->Add(player);

	unsigned short id = rand() % 255;
	while(app->m_clientConnectionMap.count(id))
		id = rand() % 255;
	ClientConnection connection;
	connection.id = id;
	connection.player = player;

	for(map<unsigned long, ClientConnection *>::iterator it = app->m_clientConnectionMap.begin(); it != app->m_clientConnectionMap.end(); it++)
	{
		ClientConnection *con = it->second;
		con->creationQueue.push(pair<unsigned long, game2d::PhysicalObject *>((unsigned long)id, player));
		connection.creationQueue.push(pair<unsigned long, game2d::PhysicalObject *>(con->id, con->player));
	}

	app->m_clientConnectionMap[id] = &connection;
	g_physicsMutex.Unlock();
	print("New CL: %d\n", id);

	string packetPrefix = "P:";
	string readBuffer;

	unsigned long lastPositionUpdate = app->GetRunTimeMS();

	int hasRead = 0;
	while(true)
	{
		unsigned long t = app->GetRunTimeMS();

		hasRead = client->HasRead();
		if(hasRead > 0)
		{
			do
			{
				char buffer[257];
				memset(buffer, 0, 256);
				unsigned short ret = client->Read(buffer, 256);
				readBuffer.append(buffer, ret);
			}
			while(client->HasRead() > 0);
		}
		else if(hasRead < 0)
		{
			cout << "Connection closed" << endl;
			break;
		}

		while(readBuffer.size() >= 32)
		{
			size_t fP = readBuffer.find_first_of("P:");
			if(fP != 0)
			{
				if(fP == string::npos)
					readBuffer.clear();
				else
					readBuffer = readBuffer.substr(fP);
			}

			if(readBuffer.size() < 32)
				break;

			Packet response;
			memcpy(&response, &readBuffer[2], sizeof(Packet));

			if(response.type == Movement)
			{
//				cout << "Movement: " << response.movement.velX << " " << response.movement.velY << endl;
				g_physicsMutex.Lock();
				player->SetVelocity(Vector2<float>(response.movement.velX, response.movement.velY));
				g_physicsMutex.Unlock();
			}

			readBuffer = readBuffer.substr(32);
		}

		if((t - lastPositionUpdate) >= 32)
		{
			lastPositionUpdate = t;

			// objects need id's derp
	//		vector<Group::Member *> &groupMembers = app->m_group->GetMembers();
	//		for(vector<Group::Member *>::iterator it = groupMembers.begin(); it != groupMembers.end(); it++)
			for(map<unsigned long, ClientConnection *>::iterator it = app->m_clientConnectionMap.begin(); it != app->m_clientConnectionMap.end(); it++)
			{
				ClientConnection *clientConnection = it->second;
				game2d::PhysicalObject *obj = (game2d::PhysicalObject *)clientConnection->player;

				Vector2<float> position = obj->GetPosition();
				Vector2<float> velocity = obj->GetVelocity();
				float rotation = obj->GetRotation();
				Packet update;
				update.type = Update;
				if(obj == player)
					update.update.objID = 0;
				else
					update.update.objID = clientConnection->id;
				update.update.posX = position[0];
				update.update.posY = position[1];
				update.update.velX = velocity[0];
				update.update.velY = velocity[1];
				update.update.rot = rotation;
				string tempPacket(packetPrefix);
				tempPacket.append((char *)&update, sizeof(Packet));
				client->Write((char *)tempPacket.c_str(), tempPacket.size());
			}
		}

		while(connection.creationQueue.size())
		{
			pair<unsigned long, game2d::PhysicalObject *> objData = connection.creationQueue.front();
			connection.creationQueue.pop();

			Vector2<float> position = objData.second->GetPosition();

			Packet creation;
			creation.type = New;
			creation.n.objID = objData.first;
			creation.n.posX = position[0];
			creation.n.posY = position[1];

			string tempPacket(packetPrefix);
			tempPacket.append((char *)&creation, sizeof(Packet));
			client->Write((char *)tempPacket.c_str(), tempPacket.size());
//			print("%d sending creation event for: %d %d\n", id, objData.first, creation.type);
		}

		while(connection.deletionQueue.size())
		{
			Packet deletion;
			deletion.type = Delete;
			deletion.del.objID = connection.deletionQueue.front();
			connection.deletionQueue.pop();
			string tempPacket(packetPrefix);
			tempPacket.append((char *)&deletion, sizeof(Packet));
			client->Write((char *)tempPacket.c_str(), tempPacket.size());
		}

		sleepMS(1);
	}

	cout << "Client Quit" << endl;

	app->m_clientConnectionMap.erase(id);
	for(map<unsigned long, ClientConnection *>::iterator it = app->m_clientConnectionMap.begin(); it != app->m_clientConnectionMap.end(); it++)
		it->second->deletionQueue.push(id);	

	g_physicsMutex.Lock();
	app->m_group->Remove(player);
	player->Kill();
	g_physicsMutex.Unlock();

	delete client;

	Thread::Exit();
	return 0;
}

int main(int argc, char **argv)
{
	ioMutexInit();
	print("551 - 2D Server | Centhra Engine v%s\n", getVersionString().c_str());

	AppTest myApp;
	myApp.Start();

	g_physicsMutex.Init();
	Thread physicsThread(physicsFunc);
	physicsThread.Start(&myApp);

	Socket *server = Socket::Create(Socket::IP4, Socket::Stream, Socket::TCP, false);
	server->Bind(12346);
	server->Listen(16);
	vector<Thread *> clientThreads;

	//- Run the App's main loop. -
	while(myApp.IsRunning())
	{
		myApp.Process();
		Socket *client = server->Accept();

		if(client)
		{
			pair<AppTest *, Socket *> *data = new pair<AppTest *, Socket *>(&myApp, client);
			Thread *clientThread = new Thread(clientFunc);
			clientThreads.push_back(clientThread);
			clientThread->Start(data);
		}

		sleepMS(16);
	}

	physicsThread.Join();
	for(vector<Thread *>::iterator it = clientThreads.begin(); it != clientThreads.end(); it++)
	{
		Thread *thread = *it;
		thread->Join();
		delete thread;
	}

	g_physicsMutex.Destroy();
	delete server;

	ioMutexDestroy();
	Thread::Exit();
	return 0;
}
