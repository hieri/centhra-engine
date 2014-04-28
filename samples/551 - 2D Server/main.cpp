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

#define NUMRANDOMS 256 

using namespace ce;
using namespace std;

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

Mutex g_physicsMutex;

class ClientConnection;

class App2DServer : public AppFrontend
{
	unsigned long long m_lastProcess;
	game2d::PhysicalObject **m_randoms;

public:
	game2d::PhysicalGroup *m_group;
	game2d::DefaultPhysicsHandler *m_defaultPhysicsHandler;
	plugin::box2d::bPhysicsHandler *m_box2dPhysicsHandler;
	map<unsigned long, ClientConnection *> m_clientConnectionMap;

	App2DServer()
	{
		m_group = 0;
		m_defaultPhysicsHandler = 0;
		m_box2dPhysicsHandler = 0;
		m_lastProcess = 0;
		m_randoms = 0;
	}
	~App2DServer()
	{
	}

	//- Define the virtual functions for the class. -
	bool OnStart()
	{
		m_group = new game2d::PhysicalGroup();
//		m_defaultPhysicsHandler = new game2d::DefaultPhysicsHandler();
//		m_group->AttachHandler(m_defaultPhysicsHandler);

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

			m_randoms[a] = new game2d::PhysicalObject(Vector2<float>((float)rx * 16.f, (float)ry * 16.f), Vector2<float>(32.f, 32.f));

			Vector2<float> dif = Vector2<float>((float)(rand() % 1024 - 512), (float)(rand() % 1024 - 512));
			m_randoms[a]->SetVelocity(dif);
			m_group->Add(m_randoms[a]);
		}

		m_box2dPhysicsHandler = new plugin::box2d::bPhysicsHandler();
		m_group->AttachHandler(m_box2dPhysicsHandler);
		return true;
	}
	void ProcessPhysics(float dt)
	{
		g_physicsMutex.Lock();
		Vector2<float> origin = Vector2<float>(512.f, 512.f);
		game2d::Entity::DeleteDead();
		for(int a = 0; a < NUMRANDOMS; a++)
		{
			Vector2<float> pos = m_randoms[a]->GetPosition();
//			Vector2<float> vel = Vector2<float>((float)(rand() % 512 - 256), (float)(rand() % 512 - 256));
//			Vector2<float> vel = m_randoms[a]->GetVelocity();
//			float length = vel.GetLength();
//			if(length)
//				vel /= length;
//			vel *= 256.f;
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
		g_physicsMutex.Unlock();
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
		m_group->DetachHandler();
		delete m_defaultPhysicsHandler;
		delete m_box2dPhysicsHandler;
		delete m_group;

		for(int a = 0; a < NUMRANDOMS; a++)
			delete m_randoms[a];
		delete [] m_randoms;
	}

	bool OnEvent(Event &event)
	{
		return true;
	}
};

void *physicsFunc(void *arg)
{
	unsigned long long lastProcess = getRunTimeMS();

	App2DServer *app = (App2DServer *)arg;

	while(app->IsRunning())
	{
		unsigned long long t = getRunTimeMS();
		if((t - lastProcess) > 15)
		{
			float dt = (float)(t - lastProcess) / 1000.f;
			lastProcess = t;

			app->ProcessPhysics(dt);
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
	queue<game2d::PhysicalObject *> creationQueue;
	queue<unsigned short> deletionQueue;
};

// thinking of how to handle telling other clients about new's/delete's
// update also needs to include velocity
// write a generalized class for keeping track of networked objects
// get rid of need for a canvas with regards to the server
void *clientFunc(void *arg)
{
	srand((unsigned int)time(NULL));
	pair<App2DServer *, Socket *> *data = (pair<App2DServer *, Socket *> *)arg;
	Socket *client = data->second;
	App2DServer *app = data->first;
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

	bool takenControl = false;
	vector<Group::Member *> &groupMembers = app->m_group->GetMembers();
	for(vector<Group::Member *>::iterator it = groupMembers.begin(); it != groupMembers.end(); it++)
	{
		game2d::PhysicalObject *obj = (game2d::PhysicalObject *)*it;
		connection.creationQueue.push(obj);
	}
	for(map<unsigned long, ClientConnection *>::iterator it = app->m_clientConnectionMap.begin(); it != app->m_clientConnectionMap.end(); it++)
	{
		ClientConnection *con = it->second;
		con->creationQueue.push(player);
	}

	app->m_clientConnectionMap[id] = &connection;
	g_physicsMutex.Unlock();
	print("New CL: %d\n", id);

	string packetPrefix = "P:";
	string readBuffer;

	unsigned long long lastPositionUpdate = app->GetRunTimeMS();

	unsigned short packetSize = sizeof(Packet);
	int hasRead = 0;
	while(true)
	{
		unsigned long long t = app->GetRunTimeMS();

		hasRead = client->HasRead();
		if(hasRead > 0)
		{
			do
			{
				char buffer[257];
				memset(buffer, 0, 256);
				int ret = client->Read(buffer, 256);

				if(ret > 0)
					readBuffer.append(buffer, ret);
			}
			while(client->HasRead() > 0);
		}
		else if(hasRead < 0)
		{
			cout << "Connection closed" << endl;
			break;
		}

		while(readBuffer.size() >= packetSize)
		{
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

			switch(type) //- TODO: Refer to array -
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

			if(response.type == Movement)
			{
//				cout << "Movement: " << response.movement.velX << " " << response.movement.velY << endl;
				g_physicsMutex.Lock();
				player->SetVelocity(Vector2<float>(response.movement.velX, response.movement.velY));
				g_physicsMutex.Unlock();
			}

			readBuffer = readBuffer.substr(thisPacketSize + 4);
		}

		if((t - lastPositionUpdate) >= 16)
		{
			lastPositionUpdate = t;

			// objects need id's derp
//			for(map<unsigned long, ClientConnection *>::iterator it = app->m_clientConnectionMap.begin(); it != app->m_clientConnectionMap.end(); it++)
			vector<Group::Member *> &groupMembers = app->m_group->GetMembers();
			for(vector<Group::Member *>::iterator it = groupMembers.begin(); it != groupMembers.end(); it++)
			{
//				ClientConnection *clientConnection = it->second;
//				game2d::PhysicalObject *obj = (game2d::PhysicalObject *)clientConnection->player;
				game2d::PhysicalObject *obj = (game2d::PhysicalObject *)*it;

				Vector2<float> position = obj->GetPosition();
				Vector2<float> velocity = obj->GetVelocity();
				float rotation = obj->GetRotation();
				Packet update;
				update.type = Update;
				update.update.objID = obj->GetID();
				update.update.posX = position[0];
				update.update.posY = position[1];
				update.update.velX = velocity[0];
				update.update.velY = velocity[1];
				update.update.rot = rotation;
				string tempPacket(packetPrefix);
				tempPacket.append((char *)&update.type, sizeof(unsigned short));
				tempPacket.append((char *)&update, sizeof(UpdatePacket));
				client->Write((char *)tempPacket.c_str(), tempPacket.size());
			}
		}

		while(connection.creationQueue.size())
		{
			game2d::PhysicalObject *obj = connection.creationQueue.front();
			connection.creationQueue.pop();

			Vector2<float> position = obj->GetPosition();

			Packet creation;
			creation.type = New;
			creation.n.objID = obj->GetID();
			creation.n.posX = position[0];
			creation.n.posY = position[1];

			string tempPacket(packetPrefix);
			tempPacket.append((char *)&creation.type, sizeof(unsigned short));
			tempPacket.append((char *)&creation, sizeof(NewPacket));
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
			tempPacket.append((char *)&deletion.type, sizeof(unsigned short));
			tempPacket.append((char *)&deletion, sizeof(DeletePacket));
			client->Write((char *)tempPacket.c_str(), tempPacket.size());
		}

		if(!takenControl)
		{
			Packet control;
			control.type = Control;
			control.control.objID = player->GetID();

			string tempPacket(packetPrefix);
			tempPacket.append((char *)&control.type, sizeof(unsigned short));
			tempPacket.append((char *)&control, sizeof(ControlPacket));
			client->Write((char *)tempPacket.c_str(), tempPacket.size());
			takenControl = true;
		}

		sleepMS(1);
	}

	cout << "Client Quit" << endl;

	app->m_clientConnectionMap.erase(id);
	for(map<unsigned long, ClientConnection *>::iterator it = app->m_clientConnectionMap.begin(); it != app->m_clientConnectionMap.end(); it++)
		it->second->deletionQueue.push(player->GetID());	

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

	App2DServer myApp;
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
			pair<App2DServer *, Socket *> *data = new pair<App2DServer *, Socket *>(&myApp, client);
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
	return 0;
}
